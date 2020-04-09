#include "Common.h"

int main()
{
	// WinSock 초기화
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed [%d]\n", result);
		return 1;
	}
	printf("Server Start...\n");

	// 리스닝 소켓 생성
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Socket can't be created [%d]\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	
	// 바인드
	sockaddr_in serverAddress = { 0 };
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(DEFAULT_TCP_PORT);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(listenSocket, (const sockaddr*)&serverAddress, sizeof(serverAddress));
	if (result == SOCKET_ERROR)
	{
		printf("Socket can't be bound [%d]\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// 대기
	result = listen(listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		printf("Can't be listening [%d]\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	printf("Listening...\n");

	// 연결
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Can't accept the client [%d]\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	closesocket(listenSocket);

	// 에코
	printf("Echoing...\n");
	int receivedByte = 0;
	char message[BUFFER_SIZE] = { 0 };
	char* buffer = message;
	int bufferSize = BUFFER_SIZE;
	do
	{
		receivedByte = recv(clientSocket, buffer, bufferSize, NULL);
		if (receivedByte > 0)
		{
			bufferSize -= receivedByte;
			result = send(clientSocket, buffer, receivedByte, NULL);
			if (result == SOCKET_ERROR)
			{
				printf("Send failed [%d]\n", WSAGetLastError());
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}
			buffer += receivedByte;
		}
		else if (receivedByte == 0)
		{
			printf("Connection closing...\n");
		}
		else
		{
			printf("Can't receive [%d]\n", WSAGetLastError());
			closesocket(clientSocket);
			WSACleanup();
			return 1;
		}
	} while (receivedByte > 0);
	printf("Received : %s\n", message);

	// 연결 해제
	result = shutdown(clientSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("Can't shut down the client [%d]\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(clientSocket);
	WSACleanup();

	return 0;
}