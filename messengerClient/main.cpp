// messengerClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "Connection.h"

using namespace std;

#define IP "10.28.26.10"
#define DEFAULT_PORT "12345"

int main() 
{
	Connection connect;

	connect.setConnectionConfig(IP, DEFAULT_PORT);

	connect.startConnect();

	system("cls");
	cout << "Connected to server address: " << IP << endl;

	connect.startTalking();
	
	system("PAUSE");
	return 0;
}
