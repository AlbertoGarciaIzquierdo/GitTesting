#include "ClientServer.h"

void ClientServer::Accept(SOCKET listener)
{
	ClientSocket = accept(listener, NULL, NULL);
	if (ClientSocket != 0) {
		std::cout << "Somebody connected to the server" << std::endl;
		//printf("New connection , socket fd is %d , ip is : %s , port : %d\n", ss[i]);
	}
}
void ClientServer::SetContact()
{

}
void ClientServer::Receive(const SOCKET& s) {
	while (true) {
		char buffer[1024];
		int bytes_received = recv(ClientSocket, buffer, sizeof(buffer), 0);
		if (bytes_received > 0) {
			std::cout << "Data RECEIVED from block " << std::endl;
			printf("%s\n", buffer);
			send(ContactSocket, buffer, sizeof(buffer), 0);
			std::cout << "Data SENT from block " << std::endl;
		}
	}
}

void ClientServer::Init(SOCKET listener) {
	Accept(listener);
	SetThread();
}

