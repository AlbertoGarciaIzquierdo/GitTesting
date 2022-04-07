// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <chrono>
#include <future>
#include <thread>

#define PORT 8080
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)


static void Receive(SOCKET* s) {
	while (true) {
		char buffer[1024];
		for (int i = 0; i < 2; i++) {
			if (s[i] != 0) {
				int bytes_received = recv(s[i], buffer, sizeof(buffer), 0);
				if (bytes_received > 0) {
					std::cout << "Data received from block " << i << std::endl;
					printf("%s\n", buffer);
					send(s[i], buffer, sizeof(buffer), 0);
				}
			}
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

static void Accept(SOCKET listener, SOCKET* ss, sockaddr_in sa) {
	// Accept the connections
	for (int i = 0; i < 2; i++) {
		ss[i] = accept(listener, NULL, NULL);
		std::cout << "Somebody connected to the server" << std::endl;
		printf("New connection , socket fd is %d , ip is : %s , port : %d\n", ss[i], inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
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
	InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
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
	// Create a SOCKET for accepting incoming request
	
	SOCKET ClientSocket[2];
	wprintf(L"Waiting for client to connect\n");


	std::thread acceptThread(Accept ,ListenSocket, ClientSocket, service);
	std::thread recvThread(Receive, ClientSocket);
	//std::thread sendThread(Send, ClientSocket);

	acceptThread.join();
	recvThread.join();
	//sendThread.join();


	/*while (true) {
		std::cout << "Esperando a recibir informacion" << std::endl;
		int bytes_received = recv(AcceptSocket, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
		if (bytes_received > 0) {
			std::cout << "Enviando Informacion" << std::endl;
			send(AcceptSocket, buffer, sizeof(buffer), 0);
		}
	}*/
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
