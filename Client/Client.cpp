// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <winSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <Windows.h>

#define PORT 8080

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 

static void Receive(SOCKET s) {
    while (true) {
        char buffer[1024];
        int bytes_received = recv(s, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            printf("%s\n", buffer);
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

static void Connect(SOCKET s) {
    std::cout << "Ingrese IP a conectarse -> ";
    char direction[INET_ADDRSTRLEN];
    std::cin >> direction;
    std::cout << std::endl;

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(PORT);
    service.sin_addr.s_addr = inet_addr(direction);

    if (connect(s, (SOCKADDR*)&service, sizeof(service)) < 0) {
        printf("Couldnt connect to a server");
        Connect(s);
    }
    else { std::cout << "You join " << direction << std::endl; }
}

int main()
{
    
   

    // Initialize Winshock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    Connect(ClientSocket);

    std::thread recvThread(Receive, ClientSocket);
    std::thread sendThread(Send, ClientSocket);

    recvThread.join();
    sendThread.join();

    /*
    while (true) {
        Send(ClientSocket);
        Receive(ClientSocket);
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
