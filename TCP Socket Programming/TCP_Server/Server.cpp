#include "Server.h"

Server::Server(string ipAdrr, int port) : ipAddress(ipAdrr), port(port){}

Server::~Server()
{
	CleanupWinSock();
}

bool Server::InitWinSock()
{
	//WSAStartup 
	WSAData winSockData;
	if (WSAStartup(MAKEWORD(2, 2), &winSockData) != 0)
	{
		cout << "WSAStartup Failed." << endl;
		return false;
	}
	return true;
}

SOCKET Server::CreateSocket()
{
	//Create Socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket != INVALID_SOCKET)
	{
		//Setting sockaddr_in sockAddr
		sockAddr.sin_family = AF_INET; //Address family, provide interprocess communication
		sockAddr.sin_port = htons(port); //set port
		inet_pton(AF_INET, ipAddress.c_str(), &sockAddr.sin_addr);

		//Bind Socket
		if (bind(serverSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR)
		{
			//Listen
			if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
			{
				cout << "Listen Failed with Error No:" << WSAGetLastError() << endl;
				return -1;
			}
		}
		else
		{
			cout << "Bind Socket Failed with Error No:" << WSAGetLastError() << endl;
			return -1;
		}
		return serverSocket;
	}
	else
	{
		cout << "TCP Server Socket Creation Failed." << endl;
		return -1;
	}
}

void Server::Run()
{
	ofstream file;
	file.open("backup-server.txt");
	char buffer[512];
	serverSocket = CreateSocket();

	while (true)
	{
		fd_set master;	//file descriptor yang menyimpan semua socket
		FD_ZERO(&master);	//mengosongi file descriptor

		FD_SET(serverSocket, &master);	//set socket server ke file descriptor

		while (true)
		{
			fd_set copy = master;
			int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
			
			for (int i = 0; i < socketCount; i++)
			{
				SOCKET currentSocket = copy.fd_array[i];

				if (currentSocket == serverSocket)
				{
					//menerima koneksi dari client baru
					SOCKET clientSocket = accept(serverSocket, (sockaddr*)&sockAddr, nullptr);
					if (clientSocket == INVALID_SOCKET)
					{
						cout << "Accept Failed with Error No:" << WSAGetLastError() << endl;
						return;
					}
					FD_SET(clientSocket, &master);
					string welcomeMsg = "Welcome to server!";
					send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				}
				else
				{
					//menerima dan mengirim pesan ke client saat ini
					ZeroMemory(buffer, 512);

					int iRecv = recv(currentSocket, buffer, 512, 0);

					if (iRecv <= 0)	//jika tidak ada pesan
					{
						closesocket(currentSocket);
						FD_CLR(currentSocket, &master);
					}
					else
					{
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET otherSocket = master.fd_array[i];

							//cek jika otherSocket tidak sama dengan serverSocket
							if (otherSocket != serverSocket)
							{
								//jika client lain sama dengan client yang sedang mengirim pesan 
								if (otherSocket == currentSocket)
								{
									string msg = "Message delivered!";
									send(otherSocket, msg.c_str(), msg.length() + 1, 0);
								}
								else
								{
									//kirimkan pesan ke client lain 
									send(otherSocket, buffer, 512, 0);
								}
							}
						}

						cout << buffer << endl;
						file << buffer << endl;
					
					}
				}
			}
		}
	}

	file.close();
}

void Server::CleanupWinSock()
{
	closesocket(serverSocket);
	WSACleanup();
}

