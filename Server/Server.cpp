// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <thread>

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

static void Send(SOCKET s) {
	std::string message;
	while (true) {
		std::getline(std::cin, message);
		send(s, message.c_str(), sizeof(message), 0);
	}
}

/*static void Accept(SOCKET listener, SOCKET* ss, ClientServer* cs) {
	// Accept the connections
	unsigned int i = 0;
	while (true) {
		if (*ss == 0) {
			*ss = accept(listener, NULL, NULL); 
			std::cout << "Somebody connected to the server" << std::endl;
		}
	}
}*/

static void Accept(ClientServer* cliente, SOCKET listen) {
	int count = 0;
	while (true) {
		if (cliente[count].GetStatus() != 1) {
			cliente[count].Init(listen);
			count ++;
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
	InetPton(AF_INET, L"10.1.1.11", &service.sin_addr.s_addr);
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

	ClientServer MyClientServer[2];

	wprintf(L"Waiting for client to connect\n");

	std::thread AcceptThread;

	MyClientServer[0].Accept(ListenSocket);
	MyClientServer[1].Accept(ListenSocket);
	MyClientServer[0].SetThread();
	MyClientServer[1].SetThread();

	MyClientServer[0].SetContactSocket(MyClientServer[1].GetSocket());
	MyClientServer[1].SetContactSocket(MyClientServer[0].GetSocket());


	MyClientServer[0].Join();
	MyClientServer[1].Join();
	
}
