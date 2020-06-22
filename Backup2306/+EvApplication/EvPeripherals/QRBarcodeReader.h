#pragma once

#include "EthernetCommunication.h"

class CQRBarcodeReader
{
private:
	EthernetCommunication EthernetPort;

	HANDLE ReadThreadDestroyed;
	CString strPortID;

	bool bCommunicationEstablished;

	int nPortName;

	int nConnectionNo;

	HANDLE ReceivedMessage[2];
	char cReceivedData[2][8600];   //Max length of QR code with Alphanummeric is 4296 characters
	int nReceivedCommandLength[2];

	CRITICAL_SECTION CriticalSectionReceivedData;

	int nRespondTimeMs;

	int nReceivedMessageNo;

	BYTE bTerminator;

	CString strDeviceID;

	BOOL bReceiveData;

	CString ipAddress;

public:
	CQRBarcodeReader(void);
	~CQRBarcodeReader(void);

	int Initialize(const int nPortID, const CString ipAddress);
	void setQRReceivedTimeOutPeriod(int timeOut);

	void CloseConnection();

	int SendMessageToServer(CString strMessage);

	//Command Set
	int GetDeviceInfo(CString & strInfo);
	int GetDeviceScanningStatus(CString & strInfo, BOOL & bScanStatus);
	int AllOutputsON();
	int AllOutputsOFF();
	int Reset();
	int ClearBuffer();
	int LaserON();
	int LaserOFF();
	int TriggerInputON();
	int TriggerInputOFF();

private:
	//For receiving messages
	static UINT MessageReceiver(LPVOID pParam);
	void MessageReceivingThread();

	int SendAndReceiveCommand(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength);

	int SendAndReceiveCommand(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nRecievedCommandLength);

	int SendAndReceiveTwoCommands(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength);

	int SendAndReceiveTwoCommands(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nRecievedCommandLength);

	int IsValidCharacter(char cReceivedChar);
};

