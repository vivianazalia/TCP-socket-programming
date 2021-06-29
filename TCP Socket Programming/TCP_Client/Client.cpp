#include "Client.h"
#include<iostream>

Client::Client()
{
	threadRecvRunning = false;
	joinChat = true;
}

Client::~Client()
{
	CleanupWinSock();
}

bool Client::InitWinSock()
{
	//WSAStartup 
	WSADATA winSockData;
	if (WSAStartup(MAKEWORD(2, 2), &winSockData) != 0)
	{
		cout << "WSAStartup Failed." << endl;
		return false;
	}
	return true;
}

SOCKET Client::CreateSocket()
{
	//Create Socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "TCP Client Socket Creation Failed." << endl;
		WSACleanup();
		return -1;
	}

	serverAddr.sin_family = AF_INET; //Address family, provide interprocess communication
	serverAddr.sin_port = htons(serverPort); //set port
	inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr); //set IP

	return clientSocket;
}

void Client::ConnectSocket()
{
	serverSocket = CreateSocket();

	int connectResult = connect(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (connectResult == SOCKET_ERROR)
	{
		cout << "Connection failed with error number: " << WSAGetLastError() << endl;
		return;
	}

	cout << "===== " << this->username << "'s RoomChat =====" << endl;
}

void Client::ThreadRecv() 
{
	threadRecvRunning = true;
	while (threadRecvRunning)
	{
		char buffer[512];
		ZeroMemory(buffer, 512);

		int byteRecv = recv(serverSocket, buffer, 512, 0);
		
		if (byteRecv > 0)	//client disconnect byteRecv = 0, error byteRecv = -1
		{
			cout << string(buffer, 0, byteRecv) << endl;
		}
	}
}

void Client::CreateThread()
{
	th = thread([&] {ThreadRecv();});
}

void Client::SendMsg(string msg)
{
	if (!msg.empty() && serverSocket != INVALID_SOCKET)
	{
		send(serverSocket, msg.c_str(), msg.size() + 1, 0);
	}
}

void Client::CleanupWinSock()
{
	closesocket(serverSocket);
	WSACleanup();
	if (threadRecvRunning)
	{
		threadRecvRunning = false;
	}
}

void Client::SetUsername(string uname) 
{
	this->username = uname;
}

void Client::SetJoinChat(bool b)
{
	joinChat = b;
}

bool Client::GetJoinChat()
{
	return joinChat;
}

string Client::GetUsername()
{
	return username;
}