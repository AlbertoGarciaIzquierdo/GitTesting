#include "ClientServer.h"

void ClientServer::Accept()
{
	bool connected = false;
	bool status = true; // FALSE -> Sending; TRUE -> Receiving;

	while (true) {
		if (!connected) {
			wprintf(L"Waiting for client to connect\n");
			ClientSocket = accept(Listener, NULL, NULL);
			if (ClientSocket != 0) {
				std::cout << "Somebody connected to the server" << std::endl;
				connected = true;
			}
		}
		/*else {
			std::string buffer = "ITEM_READ:2022-03-21T17:04:17.487+0100:3014ED5BE056CC0000000054";
			int bytesSent = send(ClientSocket, buffer.c_str(), buffer.length(), 0);
			std::cout << "ENVIADO" << std::endl;
			if (bytesSent == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
				std::cout << "Client Disconnected" << std::endl;
				closesocket(ClientSocket);
				connected = false;
			}
		}*/

		else {
			char buffer[4096];
			int bytes_received = recv(ClientSocket, buffer, sizeof(buffer) / sizeof(buffer[0]), 0);
			if (bytes_received > 0) {
				std::cout << "Data received from block " << std::endl;
				printf("%s\n", buffer);
				send(ClientSocket, buffer, sizeof(buffer) / sizeof(buffer[0]), 0);
			}
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
