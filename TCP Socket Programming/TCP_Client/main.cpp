#include<iostream>
#include<sstream>
#include"Client.h"

void main() 
{
	Client* client = new Client;
	string inputUsername;
	string msg;

	cout << "Enter a username: "; cin >> inputUsername;
	client->SetUsername(inputUsername);

	if (client->InitWinSock())
	{
		client->ConnectSocket();
		client->CreateThread();

		while (true)
		{
			getline(cin, msg);

			string msgToSend;
			if (!client->GetJoinChat())
			{
				ostringstream ss;
				ss << client->GetUsername() << " : " << msg;
				msgToSend = ss.str();
			}
			else
			{
				ostringstream ss;
				ss << client->GetUsername() << " joined the chat.";
				msgToSend = ss.str();
				client->SetJoinChat(false);
			}
			client->SendMsg(msgToSend);
		}
	}

	delete client;
}
