#pragma once

// Ihar, June 2014
// RS-232 communication

#include <Windows.h>

class CRS232Communication
{
private:
	HANDLE SerialEvent;

	bool bOpenedPort;
	bool bDataReady;

	bool bReceiveReady;
	bool bSendReady;

	CString strPortName;

	OVERLAPPED osReceive;
	OVERLAPPED osSend;

public:
	CRS232Communication(void);
	~CRS232Communication(void);

	int OpenPort(int nPortID, long lBaudRate, bool bParity = true, int nCharLength = 8);

	int ClosePort();

	int WriteData(BYTE * pbMessage, int nBytesToWrite, int & nBytesSent);
	int ReadData(BYTE * pbMessage, int nBytesToRead, int & nBytesFromRead);
};

