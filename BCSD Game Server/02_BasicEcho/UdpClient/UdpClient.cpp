#include "Common.h"

#include <string>

int main()
{
	// Winsock 초기화
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed! [%d]\n", result);
		return 1;
	}
	printf("Welcome!\n");
	
	// 소켓 생성
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET)
	{
		printf("A socket can't be created [%d]\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// 데이터 송신
	char message[BUFFER_SIZE] = { 0 };
	printf("Enter the message : ");
	gets_s(message);

	addrinfo* serverAddress = nullptr;
	addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	result = getaddrinfo(NULL, std::to_string(DEFAULT_UDP_PORT).c_str(), &hints, &serverAddress);
	if (result != NO_ERROR)
	{
		printf("Can't find the server [%d]\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 1;
	}
	
	result = sendto(s, message, strlen(message), NULL, serverAddress->ai_addr, serverAddress->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("Can't send the message [%d]\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 1;
	}

	// 데이터 수신
	result = recvfrom(s, message, sizeof(message), NULL, NULL, NULL);
	if (result == SOCKET_ERROR)
	{
		printf("Can't receive the message [%d]\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 1;
	}
	printf("Received : %s\n", message);

	// 연결 해제
	closesocket(s);
	WSACleanup();
	
	return 0;
}
