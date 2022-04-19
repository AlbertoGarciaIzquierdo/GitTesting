#include "ClientServer.h"

void ClientServer::Accept()
{
	bool connected = false;

	while (true) {
		if (!connected) {
			wprintf(L"Waiting for client to connect\n");
			ClientSocket = accept(Listener, NULL, NULL);
			if (ClientSocket != 0) {
				std::cout << "Somebody connected to the server" << std::endl;
				//printf("New connection , socket fd is %d , ip is : %s , port : %d\n", ss[i]);
				connected = true;
			}
		}
		else {
			std::string buffer = "ITEM_READ:2022-03-21T17:04:17.487+0100:3014ED5BE056CC0000000054";
			int bytesSent = send(ClientSocket, buffer.c_str(), buffer.length(), 0);
			if (bytesSent == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
				std::cout << "Client Disconnected" << std::endl;
				closesocket(ClientSocket);
				connected = false;
			} else if (bytesSent > 0) { Sleep(1000); }
		}
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

void ClientServer::Send() {
	while (true) {
		std::string buffer = "ITEM_READ:2022-03-21T17:04:17.487+0100:3014ED5BE056CC0000000054";
		int bytesSent = send(ClientSocket, buffer.c_str(), buffer.length(), 0);
		if (bytesSent == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
			std::cout << "Client Disconnected" << std::endl;
			closesocket(ClientSocket);
		}
		Sleep(1000);
		}
	}
