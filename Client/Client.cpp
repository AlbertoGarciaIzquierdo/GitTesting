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

struct Usuario {
    std::string name { "null" };
    std::string conversation{ "null" };

    Usuario(){}
    Usuario(std::string n, std::string c) : name(n), conversation(c){}
};

static void Receive(SOCKET s) {
    while (true) {
        char buffer[4096];
        int bytes_received = recv(s, buffer, sizeof(buffer)/sizeof(buffer[0]), 0);
        if (bytes_received > 0) {
            printf("%s\n", buffer);
        }
    }
}

static void Send(SOCKET s) {
    char message[4096];
    while (true) {
        std::cin.getline(message, 4096);
        send(s, message, sizeof(message), 0);
    }
}

static void Connect(SOCKET s) {
    std::cout << "Type Server IP -> ";
    char direction[INET_ADDRSTRLEN];
    std::cin >> direction;
    std::cout << std::endl;

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(8080);
    service.sin_addr.s_addr = inet_addr(direction);

    if (connect(s, (SOCKADDR*)&service, sizeof(service)) < 0) {
        printf("Couldnt connect to a server\n");
        Connect(s);
    }
    else {
        /*Usuario user;
        std::cin >> user.name;
        std::cin >> user.conversation;
        send(s, user.name.c_str(), sizeof(user.name),0);*/
        std::cout << "You join " << direction << std::endl;
    }
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

    shutdown(ClientSocket, 2);
    closesocket(ClientSocket);
}