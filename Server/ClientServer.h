#pragma once

#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <thread>

struct Usuario {
	std::string userName{"null"};
	std::string contact{"null"};
};

class ClientServer {
private:
	Usuario user;
	sockaddr_in address;
	int status = 0;
public:
	SOCKET ClientSocket;
	SOCKET ContactSocket;
	std::thread recvThread;

	// GETers SETters USUARIO
	Usuario GetUser() { return user; }
	void SetUserName(std::string a) { user.userName = a; }
	void SetUserContact(std::string a) { user.contact = a; }

	//GET SET STATUS
	int GetStatus() { return status; }

	// GET SOCKET
	SOCKET GetSocket() { return ClientSocket; }
	void SetClientSocket(SOCKET a) { ClientSocket = a; }
	SOCKET GetContactSocket() { return ContactSocket; }
	void SetContactSocket(SOCKET a) { ContactSocket = a; }

	// GET ADDRESS
	sockaddr_in GetAddress() { return address; }


	void Init(SOCKET listener);

	void Accept(SOCKET listener);
	void SetContact();
	void Receive(const SOCKET& s);

	void TestMethod(std::string a) { std::cout << "Soy un test method " << a << std::endl; }
	void SetThread() {
		recvThread = std::thread{ &ClientServer::Receive, this, ContactSocket};
	}
	void Join() { recvThread.join(); }
};