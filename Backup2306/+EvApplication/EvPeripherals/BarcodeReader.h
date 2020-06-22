#pragma once

#include "RS232Communication.h"

class CBarcodeReader
{
public:
	CBarcodeReader(void);
	~CBarcodeReader(void);

	int Initialize(const int nPortID);

	void CloseConnection();

	int SendMessageToServer(CString strMessage);

	//Command Set
	int LaserON();
	int LaserOFF();
	int BuzzerON();

private:
	CRS232Communication ComPort;

	HANDLE ReadThreadDestroyed;
	CString strPortID;

	int nBaudRate;
	CString strControllerType;

	bool bCommunicationEstablished;

	int nConnectionNo;

	int nPortName;
	bool bParity;
	int nCharLength;

	HANDLE ReceivedMessage[10];

	char cReceivedData[10][1048576];
	int nReceivedCommandLengths[10];

	CRITICAL_SECTION CriticalSectionReceivedData;

	int nRespondTimeMs;

	int nReceivedMessageNo;

	BYTE bHeader;
	BYTE bTerminator;

	CString strDeviceID;

	//For receiving messages
	static UINT MessageReceiver(LPVOID pParam);

	void MessageReceivingThread();

	int SendAndReceiveCommand(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength);

	int SendAndReceiveCommand(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nReceivedCommandLength);

	int SendAndReceiveTwoCommands(CString strCommand, char * pcReceivedCommand, int & nRecievedCommandLength);

	int SendAndReceiveTwoCommands(const char * pcMessage, const int nMessageLength, char * pcReceivedCommand, int & nReceivedCommandLength);
};

