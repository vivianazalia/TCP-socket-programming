#pragma once
#include<WS2tcpip.h>
#include<iostream>
#include<fstream>

using namespace std;

class Server
{
private:
	SOCKET CreateSocket();
	SOCKET serverSocket;
	sockaddr_in sockAddr;
	string ipAddress;
	int port;
public:
	Server(string ipAdrr, int port);
	~Server();
	bool InitWinSock();
	void Run();
	void CleanupWinSock();
};

