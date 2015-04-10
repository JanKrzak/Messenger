#pragma once
#include <iostream>
#include <sstream>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512

class Connection
{
public:
	Connection(void);
	~Connection(void);

	void setConnectionConfig(std::string ip, std::string serv_port);
	void startConnect();
	void startTalking();

private:
	void initalizeWinsock();
	
	WSADATA _wsaData;

	char* _ipAddres;
	char* _port;
	int _iResult;
	char* _sendbuf;
	char _userName[512];

};

