// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <thread>

#define PORT 8080
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)


static void Receive(SOCKET s, char* b) {
	int valread = recv(s, b, sizeof(b), 0);
	printf("%s\n", b);
}

static void Send(SOCKET s) {
	char message[255];
	while (true) {
		std::cin.getline(message, sizeof(message));
		send(s, message, sizeof(message), 0);
	}
}


int main()
{
	char buffer[1024] {0};




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
	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// Create a SOCKET for accepting incoming request
	SOCKET AcceptSocket;
	wprintf(L"Waiting for client to connect...\n");

	// Accept the connections
	AcceptSocket = accept(ListenSocket, NULL, NULL);

	/*std::thread RecvThread(Receive, AcceptSocket, buffer);
	std::thread SndThread(Send, AcceptSocket);*/

	while (true) {
		int valread = recv(AcceptSocket, buffer, sizeof(buffer), 0);
		printf("%s\n", buffer);
		//SndThread.join();
	}
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
