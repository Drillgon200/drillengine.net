#pragma once

#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

namespace TCPTest {

void tcp_send_data(SOCKET connection, const void* data, uint32_t length) {
	//std::cout << "< ";
	for (uint32_t i = 0; i < length; i++) {
	//	std::cout << reinterpret_cast<const char*>(data)[i];
	}
	//std::cout << '\n';
	if (send(connection, reinterpret_cast<const char*>(data), length, 0) == SOCKET_ERROR) {
		int lastErr = WSAGetLastError();
		closesocket(connection);
	}
}

void tcp_send_str(SOCKET connection, const char* str) {
	tcp_send_data(connection, str, strlen(str) + 1);
}

uint32_t tcp_receive_data(SOCKET connection, void* buffer, uint32_t bufferCapacity) {
	int dataReceived = recv(connection, reinterpret_cast<char*>(buffer), bufferCapacity, 0);
	uint32_t dataReceivedU32 = uint32_t(dataReceived);
	if (dataReceived == SOCKET_ERROR) {
		int lastErr = WSAGetLastError();
		closesocket(connection);
	} else {
		std::cout << "> ";
		for (uint32_t i = 0; i < dataReceivedU32; i++) {
			std::cout << reinterpret_cast<char*>(buffer)[i];
		}
		std::cout << '\n';
		std::cout << "Data received\n";
	}
	return dataReceivedU32;
}

SOCKET connect_to_server(const char* address, const char* port) {
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		return NULL;
	}
	SOCKET serverSocket;
	addrinfo hints{};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	addrinfo* addrResult;
	addrinfo* oldResult;
	if (getaddrinfo(address, port, &hints, &addrResult) != 0) {
		return NULL;
	}

	oldResult = addrResult;

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		freeaddrinfo(addrResult);
		return NULL;
	}

	int err;
	if ((err = connect(serverSocket, oldResult->ai_addr, oldResult->ai_addrlen)) == SOCKET_ERROR) {
		int lastErr = WSAGetLastError();
		closesocket(serverSocket);
		freeaddrinfo(addrResult);
		return NULL;
	}
	freeaddrinfo(addrResult);
	return serverSocket;
}

void test_tcp(const char* address, const char* port) {
	SOCKET serverSocket = connect_to_server(address, port);
	uint32_t testlen = 0;
	//setsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<char*>(&testlen), 4);
	if (serverSocket == NULL) {
		std::cout << "Error: socket init failed";
		return;
	}

	char receiveBuffer[64]{};
	const uint32_t bigSize = 1024 * 1024;
	char* big = reinterpret_cast<char*>(malloc(bigSize));
	for (uint32_t i = 0; i < bigSize; i++) {
		big[i] = (i % 26) + 'a';
	}
	big[bigSize - 1] = 0;

	uint32_t start = time(nullptr);
	std::cout << "Starting tests\n";
	for (uint32_t i = 0; i < 1; i++) {
		//tcp_send_str(serverSocket, "What are we going to do tonight?\n");
		tcp_send_data(serverSocket, big, bigSize);
		tcp_receive_data(serverSocket, receiveBuffer, sizeof(receiveBuffer) - 1);
	}
	//tcp_send_data(serverSocket, big, 256);
	//tcp_receive_data(serverSocket, receiveBuffer, sizeof(receiveBuffer) - 1);
	std::cout << "Tests complete\n";
	std::cout << "Seconds taken: " << (time(nullptr) - start) << '\n';
	

	closesocket(serverSocket);
	WSACleanup();
}

}