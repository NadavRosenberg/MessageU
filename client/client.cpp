/*#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include "response.h"
#include "request.h"
#include "port.h"


#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>*/

#define PORT 8080

//int handleclient(int s) {}

int main() {
	//int sock = 0, valread;
	//struct sockaddr_in serv_addr;
	//char* hello = "Hello from client";
	//char buffer[1024] = { 0 };
	//if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	//{
	//	printf("\n Socket creation error \n");
	//	return -1;
	//}

	//serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = htons(PORT);

	//// Convert IPv4 and IPv6 addresses from text to binary form
	//if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	//{
	//	printf("\nInvalid address/ Address not supported \n");
	//	return -1;
	//}

	//if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	//{
	//	printf("\nConnection Failed \n");
	//	return -1;
	//}
	//send(sock, hello, strlen(hello), 0);
	//printf("Hello message sent\n");
	//valread = read(sock, buffer, 1024);
	//printf("%s\n", buffer);
	//return 0;






	//std::cout << "Server is running.." << std::endl;

	//Port port;

	//// Initialze winsock
	//WSADATA wsadata;
	//int ret = WSAStartup(MAKEWORD(2, 2), &wsadata);
	//if (ret != 0) {
	//	std::cerr << "Can't Initialize winsock!" << std::endl;
	//	exit(0);
	//}

	//// Create a socket
	//SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (s == INVALID_SOCKET) {
	//	std::cerr << "Can't create a socket!" << std::endl;
	//	exit(0);
	//}

	//// Bind the ip address and port to the socket
	//struct sockaddr_in sa = { 0 };
	//sa.sin_addr.s_addr = INADDR_ANY;
	//sa.sin_family = AF_INET;
	//sa.sin_port = htons(port.getPort());

	//bind(s, (struct sockaddr*)&sa, sizeof(sa));

	//listen(s, SOMAXCONN);

	//std::cout << "listening on port " << port.getPort() << std::endl;

	//while (true) {
	//	handleclient(s);
	//}

	//// Close the socket
	//closesocket(s);

	//// Cleanup winsock
	//WSACleanup();

	//return 0;
}