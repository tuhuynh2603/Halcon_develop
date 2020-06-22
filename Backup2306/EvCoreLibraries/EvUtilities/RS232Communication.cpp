#include "stdafx.h"
#include "RS232Communication.h"
#include "DebugLog.h"


CRS232Communication::CRS232Communication(void)
{
	bOpenedPort   = false;
	bDataReady    = false;
	bReceiveReady = false;
	bSendReady    = false;

	osReceive.Internal     = 0;
	osReceive.InternalHigh = 0;
	osReceive.Offset       = 0;
	osReceive.OffsetHigh   = 0;
	osReceive.Pointer      = 0;
	osReceive.hEvent       = 0;

	osSend.Internal     = 0;
	osSend.InternalHigh = 0;
	osSend.Offset       = 0;
	osSend.OffsetHigh   = 0;
	osSend.Pointer      = 0;
	osSend.hEvent       = 0;

	osReceive.hEvent = CreateEventA(0, TRUE, FALSE, NULL);
	if(osReceive.hEvent != 0)
		bReceiveReady = true;

	osSend.hEvent = CreateEventA(0, TRUE, FALSE, NULL);
	if(osSend.hEvent != 0)
		bSendReady = true;

	bDataReady = bReceiveReady && bSendReady;
}

CRS232Communication::~CRS232Communication(void)
{
	ClosePort();

	if(bReceiveReady)
		CloseHandle(osReceive.hEvent);

	if(bSendReady)
		CloseHandle(osSend.hEvent);
}

int CRS232Communication::OpenPort(int nPortID, long lBaudRate, bool bParity, int nCharLength)
{
	if(nPortID < 0) return -1;

	ClosePort();

	//1. Open the port
	if(nPortID < 10)
		strPortName.Format("COM%d", nPortID);
	else
		strPortName.Format("\\\\.\\COM%d", nPortID);

	SerialEvent = NULL;

	SerialEvent = CreateFile(strPortName, GENERIC_WRITE|GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

	strPortName.Format("COM%d", nPortID);

	if(SerialEvent == INVALID_HANDLE_VALUE || SerialEvent == NULL) {
		if(GetLastError() == ERROR_FILE_NOT_FOUND) {
			OutputDebugLogTo(0, TRUE, "RS232:: %s doesn't exist", strPortName);
			return -1;
		}
	}
	

	//2. Set communication mask
	BOOL bSuccess;

	bSuccess = SetCommMask(SerialEvent, EV_RXCHAR); // ALL characters accepted
	if(!bSuccess) {
		OutputDebugLogTo(0, TRUE, "RS232:: %s, Set Communication Mask (Error)", strPortName);
		return -1;
	}

	//3. Set communication parameters
	DCB serialParams = { 0 };

	bSuccess = GetCommState(SerialEvent, &serialParams);
	if(!bSuccess) {
		OutputDebugLogTo(0, TRUE, "RS232:: %s, Get Communication State (Error)", strPortName);
		return -1;
	}

	CString strSerialParameters;

	strSerialParameters.Format("%s: baud=%ld parity=%s data=%1d stop=1", strPortName, lBaudRate, bParity ? "E" : "N", nCharLength);

	strSerialParameters.ReleaseBuffer();

	bSuccess = BuildCommDCB(strSerialParameters, &serialParams);

	if(!bSuccess) {
		OutputDebugLogTo(0, TRUE, "RS232:: %s, Build Communication DCB (Error)", strPortName);
		return -1;
	}


	bSuccess = SetCommState(SerialEvent, &serialParams);
	if(!bSuccess) {
		OutputDebugLogTo(0, TRUE, "RS232:: %s, Set Communication State (Error)", strPortName);
		return -1;
	}

	//4. Set timeout parameters
	COMMTIMEOUTS CommTimeOuts;

	CommTimeOuts.ReadIntervalTimeout         = MAXDWORD;
    CommTimeOuts.ReadTotalTimeoutMultiplier  = 0;
    CommTimeOuts.ReadTotalTimeoutConstant    = 0;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant   = 0;

    bSuccess = SetCommTimeouts(SerialEvent, &CommTimeOuts);
	if(!bSuccess) {
		OutputDebugLogTo(0, TRUE, "RS232:: %s, Set Communication Timeout (Error)", strPortName);
		return -1;
	}

	bOpenedPort = true;

	return 0;
}

int CRS232Communication::ClosePort()
{
	if(bOpenedPort & bDataReady) {
		bOpenedPort = false;

		if(!CloseHandle(SerialEvent))
			return -1;
	}

	return 0;
}

int CRS232Communication::WriteData(BYTE * pbMessage, int nBytesToWrite, int & nBytesSent)
{
	if(nBytesToWrite < 1) return -1;
	if(pbMessage == NULL) return -1;

	DWORD dwBytesSent = 0;

	if(bOpenedPort & bDataReady) {

		BOOL bSuccess;

		bSuccess = WriteFile(SerialEvent, pbMessage, nBytesToWrite, &dwBytesSent, &osSend);
		if(!bSuccess) {

			if(GetLastError() != ERROR_IO_PENDING) {
				OutputDebugLogTo(0, TRUE, "RS232:: %s, Error in I/O", strPortName);
				return -1;
			}
			else { 
				// Write is pending
				bSuccess = GetOverlappedResult(SerialEvent, &osSend, &dwBytesSent, TRUE);
				if(!bSuccess) {
					return -1;
				}

			}
		}
		
		nBytesSent = dwBytesSent;


	}

	return 0;
}

int CRS232Communication::ReadData(BYTE * pbMessage, int nBytesToRead, int & nBytesFromRead)
{
	if(nBytesToRead < 1)  return -1;
	if(pbMessage == NULL) return -1;

	nBytesFromRead = 0;

	if(bOpenedPort & bDataReady) {
	
		DWORD dwEventMask;
		BOOL bSuccess;

		bSuccess = WaitCommEvent(SerialEvent, &dwEventMask, NULL);
		if(!bSuccess) {
			
			if(bOpenedPort) {
				OutputDebugLogTo(0, TRUE, "RS232:: %s, Communication error", strPortName);
				return -1;
			}
			else {
				return 0;
			}

		}

		if(dwEventMask & EV_RXCHAR) {
			DWORD dwBytesFromRead = 0;
		
			bSuccess = ReadFile(SerialEvent, (void*)pbMessage, nBytesToRead, &dwBytesFromRead, &osReceive);
			if(!bSuccess) {
				if(GetLastError() != ERROR_IO_PENDING) {
					OutputDebugLogTo(0, TRUE, "RS232:: %s, Error in I/O", strPortName);
					return -1;
				}
				else {
					// Read is pending
					bSuccess = GetOverlappedResult(SerialEvent, &osReceive, &dwBytesFromRead, TRUE);
					if(!bSuccess) {
						return -1;
					}

				}
			}

			nBytesFromRead = dwBytesFromRead;
		}

	}

	return 0;
}