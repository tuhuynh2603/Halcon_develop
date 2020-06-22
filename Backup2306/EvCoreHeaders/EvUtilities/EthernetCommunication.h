#pragma once

// Ihar, January 2016
// Ethernet communication

class EthernetCommunication
{
public:
	EthernetCommunication();
	~EthernetCommunication();

	int OpenPort(bool server, int portId, const char * ipAddress = NULL);
	void ClosePort();

	int WriteData(const char * messagePtr, int bytesToWrite, int & bytesSent) const;
	int ReadData(char * messagePtr, int bytesToRead, int & bytesFromRead) const;

	bool IsServer() const;

	bool IsPortOpened() const;

private:
	bool openedPort;

	bool m_server;

	SOCKET SocketID;

	CString portName;

	SOCKET ServerSocket;

	bool waitForConnection;

	int OpenServerPort();
	int OpenClientPort(const char * ipAddress);
};
