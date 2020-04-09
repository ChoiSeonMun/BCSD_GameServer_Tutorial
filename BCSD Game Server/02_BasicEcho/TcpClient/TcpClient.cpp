#include "Common.h"

#include <string>

int main()
{
	// Winsock 초기화
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup Failed: %d\n", result);
		return 1;
	}
	
	// 서버 주소 획득
	printf("Attempting to connect...\n");
	addrinfo hints = { 0 };
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* serverAddress = nullptr;
	// pNodeName에 NULL, "localhost", "127.0.0.1" 모두
	// 본인 컴퓨터를 지칭한다.
	result = getaddrinfo(NULL, std::to_string(DEFAULT_TCP_PORT).c_str(), &hints, &serverAddress);
	if (result != 0)
	{
		printf("Can't find the server : %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// 연결
	SOCKET connectSocket = socket(
		serverAddress->ai_family, serverAddress->ai_socktype,
		serverAddress->ai_protocol);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Socket can't be created : %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	result = connect(connectSocket, serverAddress->ai_addr, serverAddress->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("Can't connect the server : %d\n", WSAGetLastError());
		closesocket(connectSocket);
		freeaddrinfo(serverAddress);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(serverAddress);

	// 메시지 보내기
	char message[BUFFER_SIZE];
	printf("Enter the message you wanna send : ");
	gets_s(message);
	result = send(connectSocket, message, strlen(message), NULL);
	if (result == SOCKET_ERROR)
	{
		printf("Sorry. we can't send this message : %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}
	printf("Send succeeded\n");

	result = shutdown(connectSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("The socket can't be shut down with error : %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	// 메시지 받기
	int receivedByte = 0;
	int bufferLength = BUFFER_SIZE;
	char* buffer = message;
	do
	{
		receivedByte = recv(connectSocket, buffer, bufferLength, NULL);
		if (receivedByte > 0)
		{
			buffer += receivedByte;
			bufferLength -= receivedByte;
		}
		else if (receivedByte == 0)
		{
			printf("Connection Closing...\n");
		}
		else
		{
			printf("Can't receive from the server with error : %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}
	} while (receivedByte > 0);
	printf("Echoed : %s\n", message);

	// 연결 해제
	closesocket(connectSocket);
	WSACleanup();
	
	return 0;
}