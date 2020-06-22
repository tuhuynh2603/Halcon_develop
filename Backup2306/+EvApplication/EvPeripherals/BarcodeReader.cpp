#include "stdafx.h"
#include "BarcodeReader.h"

#include "DebugLog.h"

CBarcodeReader::CBarcodeReader(void)
{
	ReadThreadDestroyed			= CreateEventA(NULL, TRUE, TRUE, NULL);
	bCommunicationEstablished	= false;
	nPortName				    = 0;
	InitializeCriticalSection(&CriticalSectionReceivedData);

	for(int i=0; i<10; i++) {
		nReceivedCommandLengths[i] = -1;
		ReceivedMessage[i]	  = CreateEventA(NULL, TRUE, FALSE, NULL);
	}

	nConnectionNo = 0;

	nBaudRate = 9600;			//Default Baud Rate
	bParity = true;
	nCharLength = 8;

	nRespondTimeMs		= 1000;	//Default Time Out
	nReceivedMessageNo	= 0;

	bTerminator			= 0x0D; //[CR] : Terminator (Enter)

	strDeviceID = "";
}


CBarcodeReader::~CBarcodeReader(void)
{
	CloseConnection();

	DeleteCriticalSection(&CriticalSectionReceivedData);
}

int CBarcodeReader::Initialize(const int nPortID)
{
	if(nPortID < 1)		return -1;
	if(nPortID > 65536) return -1;

	CloseConnection();

	// 1. Setting up COM Port
	bCommunicationEstablished = true;
	nPortName				  = nPortID;

	for(int i = 0; i < 10; i++){
		nReceivedCommandLengths[i] = -1;
		memset((void*) cReceivedData[i], 0, 1048576);
	}

	strPortID.Format("COM%d",nPortName);	//Set the COMPort Name

	nConnectionNo = 1;

	// 2. Create receiver threads
	CWinThread * pWinThread;

	for(int i=0; i<nConnectionNo; i++) {
		pWinThread = NULL;

		pWinThread = ::AfxBeginThread(MessageReceiver, this, THREAD_PRIORITY_ABOVE_NORMAL);
		
		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : Error in opening port", strPortID);
			CloseConnection();
			return -1;
		}
	}

	OutputDebugLogTo(7, TRUE, "Barcode Reader: Communication Setup Completed");

	return 0;
}

void CBarcodeReader::CloseConnection()
{
	if(nConnectionNo == 1) {
		bCommunicationEstablished = false;

		ComPort.ClosePort();

		for(int i=0; i<10; i++) 
			SetEvent(ReceivedMessage[i]);

		if(WaitForSingleObject(ReadThreadDestroyed, nRespondTimeMs) != WAIT_OBJECT_0)
			OutputDebugLogTo(7, TRUE, "Barcode Reader: Receiving thread has not been destroyed");
	}

	nConnectionNo = 0;
}

int CBarcodeReader::SendMessageToServer(CString strMessage)
{
	BYTE cReceivedCommand[1024];
	int nReceivedCommandLength = 0;

	return SendAndReceiveCommand(strMessage, (char*)cReceivedCommand, nReceivedCommandLength);
}

//Command Set
int CBarcodeReader::LaserON()
{
	BYTE cReceivedCommand[1024];
	int nReceivedCommandLength = 0;

	int nError;

	CString strCommand;

	//Header[SYN] (0x16)
	strCommand.Format("[SYN]T[CR]");

	nError = SendAndReceiveCommand(strCommand, (char*)cReceivedCommand, nReceivedCommandLength);
	if(nError) return nError;

	//-- Check return value

	return 0;
}

int CBarcodeReader::LaserOFF()
{
	BYTE cReceivedCommand[1024];
	int nReceivedCommandLength = 0;

	int nError;

	CString strCommand;

	//Header[SYN] (0x16)
	strCommand.Format("[SYN]U[CR]");

	nError = SendAndReceiveCommand(strCommand, (char*)cReceivedCommand, nReceivedCommandLength);
	if(nError) return nError;

	//-- Check return value

	return 0;
}

int CBarcodeReader::BuzzerON()
{
	BYTE cReceivedCommand[1024];
	int nReceivedCommandLength = 0;

	int nError;

	CString strCommand;

	//Header [N/A], Terminator [N/A]
	strCommand.Format("[BEL]");

	nError = SendAndReceiveCommand(strCommand, (char*)cReceivedCommand, nReceivedCommandLength);
	if(nError) return nError;

	//-- Check return value

	return 0;
}


UINT CBarcodeReader::MessageReceiver(LPVOID pParam)
{
	CBarcodeReader * pBarCodeReader = (CBarcodeReader*)pParam;

	ResetEvent(pBarCodeReader->ReadThreadDestroyed);

	pBarCodeReader->MessageReceivingThread();

	OutputDebugLogTo(7, TRUE, "Barcode, %s : Message Receiver Destroyed", pBarCodeReader->strPortID);

	SetEvent(pBarCodeReader->ReadThreadDestroyed);

	return 0;
}

void CBarcodeReader::MessageReceivingThread()
{
	int nError;

	int nMessageLength = 0;

	int i, j;
	int nByteNo;

	char bData[51200];

	char bReceived[102400];

	while(bCommunicationEstablished) {
		if(!bCommunicationEstablished) return;

		nError = ComPort.OpenPort(nPortName, nBaudRate, bParity, nCharLength);

		if(!bCommunicationEstablished) return;

		if(nError) {
			OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : Error in opening port", strPortID);
			Sleep(1000);
			continue;
		}

		nMessageLength = 0;

		while(bCommunicationEstablished) {

			nError = ComPort.ReadData((BYTE*)bData, 51200, nByteNo);

			if(!bCommunicationEstablished) return;

			if(nError) {
				if(nError < 0)
					OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : READ DATA ERROR", strPortID);
			}

			//Process received data
			for(i=0; i<nByteNo; i++) {

				if(nMessageLength > 51197) {
					nMessageLength = 0;
					continue;
				}

				bReceived[nMessageLength++] = bData[i];


				if(bData[i] == bTerminator) {

					bReceived[nMessageLength] = 0;
					OutputDebugLogTo(7, TRUE, "%b", (BYTE)bReceived);

					//Check message structure
					if(!nError) {
						OutputDebugLogTo(7, TRUE, "No in thread: %d", nReceivedMessageNo);

						nReceivedCommandLengths[nReceivedMessageNo] = nMessageLength;

						for(j=0; j<nMessageLength; j++)
							cReceivedData[nReceivedMessageNo][j] = bReceived[j];

						SetEvent(ReceivedMessage[nReceivedMessageNo]);
						nReceivedMessageNo++;
					}
					else {
						OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : Bad Message Structure: %d", strPortID , nError);
					}

					nMessageLength = 0;
				}
			}
		}
	}

}

int CBarcodeReader::SendAndReceiveCommand(CString strCommand, char * pcReceivedCommand, int & nReceivedCommandLength)
{
	int i;

	if(strCommand.IsEmpty()) return -1;

	int nMessageLength = strCommand.GetLength();

	char * pcMessage = new char [nMessageLength];

	for(i=0; i<nMessageLength; i++)
		pcMessage[i] = strCommand.GetAt(i);

	int nError = SendAndReceiveCommand(pcMessage, nMessageLength, pcReceivedCommand, nReceivedCommandLength);

	delete [] pcMessage;

	return nError;
}

int CBarcodeReader::SendAndReceiveCommand(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nReceivedCommandLength)
{
	if(!bCommunicationEstablished)										return -1;
	if(!ComPort.OpenPort(nPortName, nBaudRate, bParity, nCharLength))	return -1;
	if(pcMessage == NULL)												return -1;
	if(nMessageLength < 1)												return -1;
	if(pcReceivedCommand == NULL)										return -1;

	int nError;
	int nBytesWritten;

	char* pcSendMessage = new char [nMessageLength+2];
	memcpy((void*)pcSendMessage, (void*)pcMessage, nMessageLength*sizeof(char));
	pcSendMessage[nMessageLength]   = bTerminator;
	pcSendMessage[nMessageLength+1] = 0;

	OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : Sending message %s", strPortID, pcSendMessage);

	nReceivedMessageNo = 0;
	ResetEvent(ReceivedMessage[0]);

	nError = ComPort.WriteData((BYTE*)pcSendMessage, nMessageLength+1, nBytesWritten);

	delete [] pcSendMessage;

	if(nError) return nError;

	if(nMessageLength+1 != nBytesWritten) {
		OutputDebugLogTo(7, TRUE, "Barcode Reader, %s : Sending command Error, %d of %d sent bytes", strPortID, nBytesWritten, nMessageLength+1);
		return -1;
	}

	if(WaitForSingleObject(ReceivedMessage[0], nRespondTimeMs) != WAIT_OBJECT_0) {
		OutputDebugLogTo(7, TRUE, "Distributed Computing, %s : Message receive timeout", strPortID);
		return -1;
	}

	if(!bCommunicationEstablished) return 0;

	EnterCriticalSection(&CriticalSectionReceivedData);

	nReceivedCommandLength = nReceivedCommandLengths[0];

	for(int i=0; i<nReceivedCommandLength; i++)
		pcReceivedCommand[i] = cReceivedData[0][i];

	LeaveCriticalSection(&CriticalSectionReceivedData);

	return 0;
}

int CBarcodeReader::SendAndReceiveTwoCommands(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength)
{
	if(strCommand.IsEmpty()) return -1;

	int nMessageLength = strCommand.GetLength();

	char * pcMessage = new char [nMessageLength];

	for(int i=0;i<nMessageLength;i++)
		pcMessage[i] = strCommand.GetAt(i);

	int nError = SendAndReceiveTwoCommands(pcMessage, nMessageLength, pcReceivedCommand, nRecievedCommandLength);

	delete [] pcMessage;

	return nError;
}

int CBarcodeReader::SendAndReceiveTwoCommands(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nRecievedCommandLength)
{
	if(!bCommunicationEstablished)										return -1;
	if(!ComPort.OpenPort(nPortName, nBaudRate, bParity, nCharLength))	return -1;
	if(pcMessage == NULL)												return -1;
	if(nMessageLength < 1)												return -1;
	if(pcReceivedCommand == NULL)										return -1;

	int nError = 0;
	int nBytesWritten;

	char * pcSendMessage = new char [nMessageLength+2];
	memcpy((void*)pcSendMessage, (void*)pcMessage, nMessageLength*sizeof(char));
	pcSendMessage[nMessageLength] = bTerminator;
	pcSendMessage[nMessageLength+1] = 0;

	OutputDebugLogTo(4, TRUE, "Barcode Reader, Port (%s) : Sending message %s", strPortID, pcSendMessage);

	ResetEvent(ReceivedMessage[0]);
	ResetEvent(ReceivedMessage[1]);
	nReceivedMessageNo = 0;

	nError = ComPort.WriteData((BYTE*)pcSendMessage, nMessageLength+1, nBytesWritten);

	CString strSendMessage = pcSendMessage;

	delete [] pcSendMessage;

	if(nError) return nError;

	if(nMessageLength+1 != nBytesWritten) {
		OutputDebugLogTo(4, TRUE, "Barcode Reader, Port (%s) : Sending command Error, %d of %d sent bytes", strPortID, nBytesWritten, nMessageLength+1);
		return -1;
	}

	//Receive First Command
	if(WaitForSingleObject(ReceivedMessage[0], nRespondTimeMs) != WAIT_OBJECT_0) {
		OutputDebugLogTo(4, TRUE, "Distributed Computing, Port (%s) : Message receive timeout", strPortID);
		return 1;
	}

	if(!bCommunicationEstablished) return 0;

	EnterCriticalSection(&CriticalSectionReceivedData);

	nRecievedCommandLength = nReceivedCommandLengths[0];
	
	for(int i=0;i<nRecievedCommandLength;i++)
		pcReceivedCommand[i] = cReceivedData[0][i];

	ResetEvent(ReceivedMessage[0]);

	LeaveCriticalSection(&CriticalSectionReceivedData);


	CString strOK;
	CString strER;
	CString strError;
	CString strReceivedCommandString;
	CString strReceivedCommandStringFull;
	BOOL bOK = TRUE;
	BOOL bER = TRUE;

	strOK.Format("OK,%s", strSendMessage);
	strER.Format("ER,%s", strSendMessage);
	
	//Get the Full Received message
	for(int i=0;i<nRecievedCommandLength;i++)
		strReceivedCommandStringFull.AppendChar(pcReceivedCommand[i]);

	//Get only the Status Message
	if(nRecievedCommandLength > nMessageLength+3) {
		for(int i=0;i<nMessageLength+3;i++)
			strReceivedCommandString.AppendChar(strReceivedCommandStringFull.GetAt(i));
	}
	else
		strReceivedCommandString = strReceivedCommandStringFull;
	
	OutputDebugLogTo(4, TRUE, "Char Message 2: -%s-", strReceivedCommandString);
	OutputDebugLogTo(4, TRUE, "Char Error   2: -%s-", strER);

	for(int i=0;i<nMessageLength+3;i++) {
		if(strReceivedCommandString[i] != strOK[i])
			bOK = FALSE;
	}

	for(int i=0;i<nMessageLength+3;i++) {
		if(strReceivedCommandString[i] != strER[i])
			bER = FALSE;
	}
		
	if(bOK) {
		//Receive Second Command
		if(WaitForSingleObject(ReceivedMessage[1], nRespondTimeMs) != WAIT_OBJECT_0) {
			OutputDebugLogTo(4, TRUE, "Distributed Computing, Port (%s) 2: Message receive timeout", strPortID);
			return 1;
		}

		else /*if(!nError)*/ {
			EnterCriticalSection(&CriticalSectionReceivedData);

			nRecievedCommandLength = nReceivedCommandLengths[nReceivedMessageNo-1];

			for(int i=0;i<nRecievedCommandLength;i++)
				pcReceivedCommand[i] = cReceivedData[nReceivedMessageNo-1][i];

			ResetEvent(ReceivedMessage[1]);

			LeaveCriticalSection(&CriticalSectionReceivedData);

			nError = 0;

		}
	}
	else if(bER) {//strReceivedCommandString.GetString() == strER.GetString()) {
		int nErrorCode = 0;
		char cErrorCode[100];
		int nCount = 0;

		for(int i = nMessageLength+4;i < nRecievedCommandLength;i++) {
			cErrorCode[nCount] = pcReceivedCommand[i];
			nCount++;
		}
		nErrorCode = atoi(cErrorCode);
		OutputDebugLogTo(4, TRUE, "Char Message: %s", cErrorCode);
		OutputDebugLogTo(4, TRUE, "Int Message: %d", nErrorCode);
		nError = nErrorCode;
	}
	else
		nError = -1;
	
	

	//-- Check Message Structure (!!!)
	OutputDebugLogTo(4, TRUE, "nError: %d", nError);
			
	return nError;
}