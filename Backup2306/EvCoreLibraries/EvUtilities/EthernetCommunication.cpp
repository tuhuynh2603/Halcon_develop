#include "stdafx.h"
#include "EthernetCommunication.h"
#include "DebugLog.h"
#include "PerformanceTimer.h"
#include <ws2tcpip.h>

EthernetCommunication::EthernetCommunication()
{
	openedPort = true;

	m_server = false;

	SocketID = INVALID_SOCKET;
	ServerSocket = INVALID_SOCKET;

	waitForConnection = false;
}

EthernetCommunication::~EthernetCommunication()
{
	ClosePort();
}

int EthernetCommunication::OpenPort(bool server, int portId, const char * ipAddress)
{
	if(portId < 0 || portId > 65535)
		return -1;

	ClosePort();

	portName.Format("%d", portId);

	m_server = server;

	if( IsServer() ) {
		int nError = OpenServerPort();
		if(nError)
			return nError;
	}
	else {
		int nError = OpenClientPort(ipAddress);
		if(nError)
			return nError;
	}

	openedPort = true;

	return 0;
}

void EthernetCommunication::ClosePort()
{
	waitForConnection = false;
	openedPort = false;

	if(ServerSocket != INVALID_SOCKET) {
		//shutdown(ServerSocket, SD_BOTH);
		closesocket(ServerSocket);
	}

	if(SocketID != INVALID_SOCKET) {
		//shutdown(ServerSocket, SD_BOTH);
		closesocket(SocketID);
	}
}

bool EthernetCommunication::IsServer() const
{
	return m_server;
}

bool EthernetCommunication::IsPortOpened() const
{
	return openedPort;
}

int EthernetCommunication::OpenServerPort()
{
	// 1. Convert communication data into socket format
	struct addrinfo * result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	
	hints.ai_flags    = AI_PASSIVE;
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int returnValue = getaddrinfo(NULL, portName, &hints, &result);
	if(returnValue != 0) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in providing protocol-independent translation from ANSI host name to address", portName);
		return -1;
	}

	// 2. Create a socket for connecting to server
	ServerSocket = INVALID_SOCKET;

	ServerSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ServerSocket == INVALID_SOCKET) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in opening socket", portName);
		freeaddrinfo(result);
		return -1;
	}

	// 3. Setup incoming connection
	returnValue = bind(ServerSocket, result->ai_addr, (int)result->ai_addrlen);
	if(returnValue == SOCKET_ERROR) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in associating local address with socket", portName);
		freeaddrinfo(result);
		closesocket(ServerSocket);
		return -1;
	}

	freeaddrinfo(result);

	returnValue = listen(ServerSocket, SOMAXCONN);
	if(returnValue == SOCKET_ERROR) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in setup incoming connection", portName);
		closesocket(ServerSocket);
		return -1;
	}

	// 4. Accept a client socket
	waitForConnection = true;

	OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Waiting for client", portName);
	SocketID = accept(ServerSocket, NULL, NULL);

	if(!waitForConnection)
		return 0;

	waitForConnection = false;

	closesocket(ServerSocket);

	if(SocketID == INVALID_SOCKET) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in giving permision an incoming connection attempt on socket", portName);
		return -1;
	}

	OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication established", portName);

	return 0;
}

int EthernetCommunication::OpenClientPort(const char * ipAddress)
{
	CPerformanceTimer openingPortTime;

	// 1. Convert communication data into socket format
	struct addrinfo * result = NULL;
	struct addrinfo hints;

	memset((void*)&hints, 0, sizeof(hints));

	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int returnValue = getaddrinfo(ipAddress, portName, &hints, &result);
	if(returnValue != 0) {
		OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in providing protocol-independent translation from ANSI host name to address", portName);
		return -1;
	}

	// 2. Try to connect to server
	struct addrinfo * ptr = NULL;
	
	OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, waiting for server", portName);

	while(1) {
		for(ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			SocketID = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if(SocketID == INVALID_SOCKET) {
				OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Error in opening socket", portName);
				freeaddrinfo(result);
				return -1;
			}

			// Connect to server
			waitForConnection = true;

			returnValue = connect(SocketID, ptr->ai_addr, (int)ptr->ai_addrlen);

			if(!waitForConnection)
				return 0;

			waitForConnection = false;

			if( openingPortTime.GetElapsedMSec() > 15000 ) {
				OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Waiting timeout for server reply", portName);
				return -1;
			}

			if(returnValue == SOCKET_ERROR) {
				closesocket(SocketID);
				SocketID = INVALID_SOCKET;
				continue;
			}

			break;
		}

		if(returnValue != SOCKET_ERROR)
			break;
	}

	freeaddrinfo(result);

	if(SocketID == INVALID_SOCKET) return -1;

	OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication established", portName);

	return 0;
}

int EthernetCommunication::WriteData(const char * messagePtr, int bytesToWrite, int & bytesSent) const
{
	if(bytesToWrite < 1 || messagePtr == NULL)
		return -1;

	if(openedPort) {

		bytesSent = send(SocketID, messagePtr, bytesToWrite, 0);

		if(!openedPort)
			return 0;

		if(bytesSent == 0) {
			OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication is closed", portName);
			return 1;
		}
		else if(bytesSent < 0) {
			OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication error", portName);
			return -1;
		}
	}

	return 0;
}

int EthernetCommunication::ReadData(char * messagePtr, int bytesToRead, int & bytesFromRead) const
{
	if(messagePtr == NULL || bytesToRead < 1)
		return -1;

	if(openedPort) {

		bytesFromRead = recv(SocketID, messagePtr, bytesToRead, 0);

		if(!openedPort)
			return 0;

		if(bytesFromRead == 0) {
			OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication is closed", portName);
			return 1;
		}
		else if(bytesFromRead < 0) {
			OutputDebugLogTo(0, TRUE, "ETHERNET:: Port %s, Communication error", portName);
			return -1;
		}
	}

	return 0;
}