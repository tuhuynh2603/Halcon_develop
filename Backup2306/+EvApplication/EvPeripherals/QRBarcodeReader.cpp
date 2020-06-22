#include "stdafx.h"
#include "QRBarcodeReader.h"

#include "DebugLog.h"


CQRBarcodeReader::CQRBarcodeReader(void)
{
	ReadThreadDestroyed       = CreateEventA(NULL, TRUE, TRUE, NULL);
	bCommunicationEstablished = true;
	nPortName                 = 0;
	InitializeCriticalSection(&CriticalSectionReceivedData);

	nReceivedCommandLength[0] = -1;
	ReceivedMessage[0]	  = CreateEventA(NULL, TRUE, FALSE, NULL);
	nReceivedCommandLength[1] = -1;
	ReceivedMessage[1]	  = CreateEventA(NULL, TRUE, FALSE, NULL);
	
	nConnectionNo = 0;

	nRespondTimeMs = 1000;
	nReceivedMessageNo = 0;

	bTerminator = 0x0D;

	strDeviceID = "";

	bReceiveData = FALSE;
}

CQRBarcodeReader::~CQRBarcodeReader(void)
{
	CloseConnection();

	DeleteCriticalSection(&CriticalSectionReceivedData);
}

UINT CQRBarcodeReader::MessageReceiver(LPVOID pParam)
{
	CQRBarcodeReader * pQRBarcodeReader = (CQRBarcodeReader*)pParam;

	ResetEvent(pQRBarcodeReader->ReadThreadDestroyed);

	pQRBarcodeReader->MessageReceivingThread();
	
	OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Message Receiver destroyed", pQRBarcodeReader->strPortID);

	SetEvent(pQRBarcodeReader->ReadThreadDestroyed);

	return 0;
}

void CQRBarcodeReader::MessageReceivingThread()
{
	int nError;

	int nMessageLength = 0;

	int i,j;
	int nByteNo;

	char bData[4300];

	char bReceived[8600];

	while(bCommunicationEstablished) {

		if(!bCommunicationEstablished) return;

		nError = EthernetPort.OpenPort(false, nPortName, ipAddress);
		
		if(!bCommunicationEstablished) return;
		
		if(nError) {
			OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Error in opening port", strPortID);
			Sleep(1000);
			continue;
		}

		OutputDebugLogTo(4, TRUE, "QR-Barcode : Communication setup completed");
	
		nMessageLength = 0;

		while(bCommunicationEstablished) {

			if(bReceiveData) {

				nError = EthernetPort.ReadData(bData, 4300, nByteNo);
				
				if(!bCommunicationEstablished) return;

				if(nError) {
					if(nError < 0)
						OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : READ DATA ERROR", strPortID);
					break;
				}

				bData[nByteNo] = '\0';

				// Process received data
				for(i=0;i<nByteNo;i++) {				
				
					if(nMessageLength > 4297) {
						nMessageLength = 0;
						continue;
					}

					if(nReceivedMessageNo > 1)
						nReceivedMessageNo = 0;

					bReceived[nMessageLength++] = bData[i];

					if(bData[i] == bTerminator) {

						bReceived[nMessageLength] = 0;

						// Check message structure (!!!)
						if(!nError) {
							//EnterCriticalSection(&CriticalSectionReceivedData);
							nReceivedCommandLength[nReceivedMessageNo] = nMessageLength;

							for(j=0; j<nMessageLength; j++)
								cReceivedData[nReceivedMessageNo][j] = bReceived[j];

							SetEvent(ReceivedMessage[nReceivedMessageNo]);
							nReceivedMessageNo++;
						}
						else {
							OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Bad Message structure: %d", nError);
						}

						nMessageLength = 0;

					}

				}
			}
		}



	}

}

void CQRBarcodeReader::CloseConnection()
{
	if(nConnectionNo == 1) {

		bCommunicationEstablished = false;
	
		EthernetPort.ClosePort();

		SetEvent(ReceivedMessage[0]);
		SetEvent(ReceivedMessage[1]);
//		SetEvent(ReceivedSecondMessage);

		if(WaitForSingleObject(ReadThreadDestroyed, nRespondTimeMs) != WAIT_OBJECT_0)
			OutputDebugLogTo(4, TRUE, "QR-Barcode : Receiving thread has not been destroyed");

	}

	nConnectionNo = 0;

	//WSACleanup();
	
}

int CQRBarcodeReader::Initialize(const int nPortID, const CString address)
{
	if(nPortID < 1)     return -1;
	if(nPortID > 65535) return -1;
	if( address.IsEmpty() || address.CompareNoCase("none") == 0)
		return -1;

	int i;

	CloseConnection();

	int nError;

	// 1. Establish connection with library
	WSADATA wsaData;

	nError = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nError) {
		OutputDebugLogTo(4, TRUE, "QR-Barcode : Error in initialization of Winsock DLL");
		return -1;
	}

	// 2. Create data
	bCommunicationEstablished = true;
	nPortName                 = nPortID;
	ipAddress				  = address;

	nReceivedCommandLength[0] = -1;
	nReceivedCommandLength[1] = -1;
	memset((void*) cReceivedData[0], 0, 8600);
	memset((void*) cReceivedData[1], 0, 8600);

	// 3. Open ports
	strPortID.Format("%d", nPortID);

	nConnectionNo = 1;

	// 4. Create receiver threads
	CWinThread * pWinThread = NULL;
	
	for(i=0;i<nConnectionNo;i++) {
		pWinThread = NULL;

		pWinThread = ::AfxBeginThread(MessageReceiver, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if(pWinThread == NULL) {
			OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Error in opening port", strPortID);
			CloseConnection();
			return -1;
		}

	}

	//OutputDebugLogTo(4, TRUE, "QR-Barcode : Communication setup completed");

	return 0;
}

void CQRBarcodeReader::setQRReceivedTimeOutPeriod(int timeOut)
{
	nRespondTimeMs = timeOut;
}

int CQRBarcodeReader::SendMessageToServer(CString strMessage)
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	return SendAndReceiveCommand(strMessage, cReceivedCommand, nReceivedCommandLegth);
}

int CQRBarcodeReader::SendAndReceiveCommand(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength)
{
	if(strCommand.IsEmpty()) return -1;

	int nMessageLength = strCommand.GetLength();

	char * pcMessage = new char [nMessageLength];

	for(int i=0;i<nMessageLength;i++)
		pcMessage[i] = strCommand.GetAt(i);

	int nError = SendAndReceiveCommand(pcMessage, nMessageLength, pcReceivedCommand, nRecievedCommandLength);

	delete [] pcMessage;

	return nError;
}

int CQRBarcodeReader::SendAndReceiveCommand(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nRecievedCommandLength)
{
	if(!bCommunicationEstablished) return -1;
	if(!EthernetPort.IsPortOpened()) return -1;
	if(pcMessage == NULL) return -1;
	if(nMessageLength < 1) return -1;
	if(pcReceivedCommand == NULL) return -1;

	int nError;
	int nBytesWritten;

	char * pcSendMessage = new char [nMessageLength+2];
	memcpy((void*)pcSendMessage, (void*)pcMessage, nMessageLength*sizeof(char));
	pcSendMessage[nMessageLength] = bTerminator;
	pcSendMessage[nMessageLength+1] = 0;

	OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Sending message %s", strPortID, pcSendMessage);
	
	nReceivedMessageNo = 0;
	ResetEvent(ReceivedMessage[0]);

	//Enable receiving
	//Need to test if it is ok to enable after sending
	bReceiveData = TRUE;

	nError = EthernetPort.WriteData((char*)pcSendMessage, nMessageLength+1, nBytesWritten);

	delete [] pcSendMessage;

	if(nError) return nError;

	if(nMessageLength+1 != nBytesWritten) {
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Sending command Error, %d of %d sent bytes", strPortID, nBytesWritten, nMessageLength+1);
		return -1;
	}

	if(WaitForSingleObject(ReceivedMessage[0], nRespondTimeMs) != WAIT_OBJECT_0) {
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Message receive timeout", strPortID);
		return 1;
	}

	//Disable message receiving
	bReceiveData = FALSE;

	if(!bCommunicationEstablished) return 0;

	EnterCriticalSection(&CriticalSectionReceivedData);

	nRecievedCommandLength = nReceivedCommandLength[0];

	for(int i=0;i<nRecievedCommandLength;i++)
		pcReceivedCommand[i] = cReceivedData[0][i];

	LeaveCriticalSection(&CriticalSectionReceivedData);

	//-- Check Message Structure (!!!)

	return 0;
}

int CQRBarcodeReader::SendAndReceiveTwoCommands(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength)
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

int CQRBarcodeReader::SendAndReceiveTwoCommands(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nRecievedCommandLength)
{
	if (!bCommunicationEstablished)
	{
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Failed to establish communication");
		return -1;
	}
	if (!EthernetPort.IsPortOpened())
	{
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Failed to open EthernetPort");
		return -1;
	}

	if (pcMessage == NULL)
	{
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Invalid message");
		return -1;
	}

	if(nMessageLength < 1) return -1;
	if (pcReceivedCommand == NULL)
	{
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Invalid ReceivedCommand");
		return -1;
	}

	int nError = 0;
	int nBytesWritten;

	char * pcSendMessage = new char [nMessageLength+2];
	memcpy((void*)pcSendMessage, (void*)pcMessage, nMessageLength*sizeof(char));
	pcSendMessage[nMessageLength] = bTerminator;
	pcSendMessage[nMessageLength+1] = 0;

	OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Sending message %s", strPortID, pcSendMessage);

	ResetEvent(ReceivedMessage[0]);
	ResetEvent(ReceivedMessage[1]);
	
	nReceivedMessageNo = 0;

	bReceiveData = TRUE;

	nError = EthernetPort.WriteData((char*)pcSendMessage, nMessageLength+1, nBytesWritten);

	CString strSendMessage = pcSendMessage;

	delete [] pcSendMessage;

	if(nError) return nError;

	if(nMessageLength+1 != nBytesWritten) {
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Port (%s) : Sending command Error, %d of %d sent bytes", strPortID, nBytesWritten, nMessageLength+1);
		return -1;
	}

	//Receive First Command
	if(WaitForSingleObject(ReceivedMessage[0], nRespondTimeMs) != WAIT_OBJECT_0) {
		OutputDebugLogTo(4, TRUE, "Qr barcode Reader, Port (%s) : Message receive timeout", strPortID);
		return 1;
	}

	if(!bCommunicationEstablished) return 0;

	EnterCriticalSection(&CriticalSectionReceivedData);

	nRecievedCommandLength = nReceivedCommandLength[0];
	
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
		DWORD dwError = WaitForSingleObject(ReceivedMessage[1], nRespondTimeMs);
		if(dwError != WAIT_OBJECT_0) {
			dwError = GetLastError();
			OutputDebugLogTo(4, TRUE, "QR Barcode Reader, Port (%s) Message 2: Error %d", strPortID, dwError);
			return 1;
		}

		else /*if(!nError)*/ {
			EnterCriticalSection(&CriticalSectionReceivedData);

			nRecievedCommandLength = nReceivedCommandLength[1];
	
			for(int i=0;i<nRecievedCommandLength;i++)
				pcReceivedCommand[i] = cReceivedData[1][i];

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
		OutputDebugLogTo(4, TRUE, "QR Barcode Reader Error Message: %s, %d", cErrorCode, nErrorCode);
		nError = nErrorCode;
	}
	else
		nError = -1;
	
	
	bReceiveData = FALSE;

	return nError;
}

int CQRBarcodeReader::GetDeviceInfo(CString & strInfo)
{
	strInfo = strDeviceID;

	return 0;
}

int CQRBarcodeReader::GetDeviceScanningStatus(CString & strTeachID, BOOL & bScanStatus)
{
	CString strDeviceID = "";
	int strBarcodeLength = strTeachID.GetLength();

	OutputDebugLogTo(4, TRUE, "QR-Barcode, Start TriggerInputON");

	int nError = TriggerInputON();

	if(!nError) {

		OutputDebugLogTo(4, TRUE, "QR-Barcode, Start GetDeviceInfo");

		GetDeviceInfo(strDeviceID);

		OutputDebugLogTo(4, TRUE, "QR-Barcode, Found GetDeviceInfo - %s", strDeviceID);

		//Compare device ID with the Teach Barcode ID
		//If Matched, proceed to scanning
		//for(int i=0;i<strBarcodeLength;i++) {
		//	if(strTeachID[i] != strDeviceID[i])
		//		bScanStatus = TRUE;
		//}
	}
	else
	{
		OutputDebugLogTo(4, TRUE, "QR-Barcode, Error in TriggerInputON - %d", nError);
	}
	
	//else
		bScanStatus = TRUE;
			
	return nError;
}

int CQRBarcodeReader::AllOutputsON()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("ALLON");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::AllOutputsOFF()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("ALLOFF");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::Reset()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("RESET");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::ClearBuffer()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("BCLR");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::LaserON()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("LDON");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::LaserOFF()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("LDOFF");

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::TriggerInputON()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError = 0;
	CString strTemp = "";

	CString strCommand;
	strCommand.Format("LON");

	nError = SendAndReceiveTwoCommands(strCommand, cReceivedCommand, nReceivedCommandLegth);
	//-- Check return value

	if(nError) {
		TriggerInputOFF();
		return -1;
	}
	else{
		strDeviceID = "";
		for(int i = 0; i < nReceivedCommandLegth; i++) {		// to eliminate special char in multi bank setting
			if(cReceivedCommand[i] != bTerminator) {

//				OutputDebugLogTo(4, TRUE, "Recv Char - %d %c", i, cReceivedCommand[i]);
				if(cReceivedCommand[i] == ':')
					break;

				//Check if any special chracters are received from the barcode reader
				if(IsValidCharacter(cReceivedCommand[i])) {
//					strDeviceID = "";

					if(cReceivedCommand[i] == '/' || cReceivedCommand[i] == '<' || cReceivedCommand[i] == '>' || cReceivedCommand[i] == '?') {
						continue;
					}
					if(cReceivedCommand[i] == '$') {
						strTemp.Format("%c", cReceivedCommand[i]);
						strDeviceID.Append(strTemp);
					}

//					return -2;
				}
				else {
					strTemp.Format("%c", cReceivedCommand[i]);
					strDeviceID.Append(strTemp);
				}
			}
		}

		// Old code
		//strDeviceID.AppendChar(cReceivedCommand[i]);
	}
	
	return 0;
}

int CQRBarcodeReader::TriggerInputOFF()
{
	char cReceivedCommand[1024];
	int nReceivedCommandLegth = 0;

	int nError;

	CString strCommand;

	strCommand.Format("LOFF");

	SetEvent(ReceivedMessage);

	nError = SendAndReceiveCommand(strCommand, cReceivedCommand, nReceivedCommandLegth);
	if(nError) return nError;

	//-- Check return value


	return 0;
}

int CQRBarcodeReader::IsValidCharacter(char cReceivedChar)
{
	return(!(isalnum(cReceivedChar)));
}
