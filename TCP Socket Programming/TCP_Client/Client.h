#pragma once
#include <string>
#include<thread>
#include<WS2tcpip.h>

using namespace std;

class Client
{
private:
	string username;
	thread th;
	const string serverIP = "127.0.0.1";
	const int serverPort = 8000;
	bool joinChat;
	bool threadRecvRunning;
	SOCKET serverSocket;
	sockaddr_in serverAddr;
	SOCKET CreateSocket();
public:
	Client();
	~Client();
	bool InitWinSock();
	void ConnectSocket();
	void ThreadRecv();
	void SendMsg(string msg);
	void CleanupWinSock();
	void SetUsername(string uname);
	void SetJoinChat(bool b);
	void CreateThread();
	bool GetJoinChat();
	string GetUsername();
};

