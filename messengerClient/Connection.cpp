#include "StdAfx.h"
#include "Connection.h"

using namespace std;

DWORD WINAPI ClientDataReciver();

//Data storage structure has information about client SOCKET
struct Data{
	SOCKET ConnectSocket;
}data;

//Addres storage structure has information about IP addres
struct addrinfo *result = NULL,
	*ptr = NULL,
	hints;

Connection::Connection(void)
{
	_sendbuf = new char[DEFAULT_BUFLEN];
}

Connection::~Connection(void)
{

}

/*
// \brief  Function set connection parameters: IP addres and port number
// \param string ip - has ip number, serv_port - has port number
*/
void Connection::setConnectionConfig(std::string ip, std::string serv_port)
{
	stringstream sIp;
	sIp << ip;
	_ipAddres = new char[10];
	sIp >> _ipAddres;
	cout<< "IP: " << _ipAddres <<endl;

	stringstream sPort;
	sPort << serv_port;
	_port  = new char [10];
	sPort >> _port;
	cout<< "Port: " << _port <<endl;
}

/*
// \brief  Function initialize Winsock
*/
void Connection::initalizeWinsock()
{
	_iResult = WSAStartup(MAKEWORD(2,2), &_wsaData);
	if (_iResult != 0) 
	{
		cout << "WSAStartup failed with error: " << _iResult <<endl;
		return;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

/*
// \brief  Function initialize connection with server
*/
void Connection::startConnect()
{
	data.ConnectSocket = INVALID_SOCKET;
	while(data.ConnectSocket == INVALID_SOCKET)
	{
		data.ConnectSocket = INVALID_SOCKET;

		initalizeWinsock();

		// Resolve the server address and port
		_iResult = getaddrinfo(_ipAddres, _port, &hints, &result);
		if ( _iResult != 0 ) 
		{
			cout << "getaddrinfo failed with error: " << _iResult <<endl;
			WSACleanup();
			return;
		}

		// Attempt to connect to an address until one succeeds
		for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
		{
			// Create a SOCKET for connecting to server
			data.ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (data.ConnectSocket == INVALID_SOCKET) 
			{
				cout << "socket failed with error:" << WSAGetLastError() <<endl;
				WSACleanup();
				return;
			}

			// Connect to server.
			_iResult = connect( data.ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (_iResult == SOCKET_ERROR)
			{
				closesocket(data.ConnectSocket);
				data.ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);
		system("cls");
		cout << "Searching for server" << endl;
	}

}

/*
// \brief  Function is used to send messages 
*/
void Connection::startTalking()
{
	cout << "Enter your name: " ;
	gets(_userName);

	send( data.ConnectSocket, _userName, (int)strlen(_userName), 0 );		//send initial message with user name to server

	HANDLE handle;
	handle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ClientDataReciver, NULL, 0, NULL);		//create new thread to listen for new messages send from server

	while(_iResult != SOCKET_ERROR)
	{
		cout << "\nTalk: ";
		gets(_sendbuf);

		// Send message
		_iResult = send( data.ConnectSocket, _sendbuf, (int)strlen(_sendbuf), 0 );
		if (_iResult == SOCKET_ERROR)
		{
			cout << "Server disconnected " <<endl;
			cout << "send failed with error: " << WSAGetLastError() <<endl;
			return;
		}
	}
}

/*
// \brief New thread function to listen for new messages from server
*/
DWORD WINAPI ClientDataReciver()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;
	bool isExit = false;

	// Receive until the server closes the connection
	while(!isExit)
	{
		iResult = recv(data.ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 )
		{
			cout<<"\r";
			for(int i = 0; i < iResult; i++)
			{
				cout << recvbuf[i];
			}
			cout<<"\n";
		}
		else
		{
			cout << "\nrecv failed with error: " << WSAGetLastError();
			isExit = true;
		}
		cout << "\nTalk: ";
	}
	return 0;
}