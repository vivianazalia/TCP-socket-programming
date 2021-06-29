#include<iostream>
#include"Server.h"

using namespace std; 

void main()
{
	cout << "=== SERVER ===" << endl;
	Server server("127.0.0.1", 8000);

	if (server.InitWinSock())
	{
		server.Run();
	}
}