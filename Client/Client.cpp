// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <winSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#define PORT 8080

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 

static void Receive(SOCKET s, char* b) {
    while (true) {
        int valread = recv(s, b, sizeof(b), 0);
        printf("%s\n", b);
    }
}

static void Send(SOCKET s) {
    char message[255];
    while (message != "exit") {
        std::cin.getline(message, sizeof(message));
        send(s, message, sizeof(message), 0);
    }
}

int main()
{
    std::cout << "Hello World!\n";
    // Initialize Winshock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(PORT);
    InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);

    if (connect(ClientSocket, (SOCKADDR*)&service, sizeof(service)) < 0) {
        printf("Couldnt connect to a server");
    }
    
    Send(ClientSocket);
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
