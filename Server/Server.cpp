// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "ClientServer.h"

#define PORT 8080
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)




static void Receive(SOCKET* r, SOCKET* s) {
	while (true) {
		char buffer[1024];
		int bytes_received = recv(*r, buffer, sizeof(buffer), 0);
		if (bytes_received > 0) {
			std::cout << "Data received from block " << std::endl;
			printf("%s\n", buffer);
			send(*s, buffer, sizeof(buffer), 0);
		}
	}
}

int main()
{
	char buffer[1024];


	// Initialize Winshock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// Create Listen socket for incoming connections
	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Create sockaddr_in structure
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"10.1.1.38", &service.sin_addr.s_addr);
	service.sin_port = htons(PORT);

	// Bind the socket to the sockaddr_in struct
	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// Listen for incoming connections
	if (listen(ListenSocket, 2) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	std::vector<ClientServer> MyClientServer(1);

	for (unsigned int i = 0; i < MyClientServer.size(); i++) {
		MyClientServer[i].SetThread(ListenSocket, MyClientServer[i].GetSocket());

		if (i == MyClientServer.size() - 1) {
			for (unsigned int j = 0; j < MyClientServer.size(); j++) {
				MyClientServer[j].Join();
			}
		}
	}






	
}
