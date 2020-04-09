#include "Common.h"

int main()
{
	// Winsock 초기화
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed [%d]\n", result);
		return 1;
	}
	printf("Server Start!\n");

	// 소켓 생성
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in address = { 0 };
	address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_UDP_PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	
	result = bind(serverSocket, (const sockaddr*)&address, sizeof(address));
	if (result == SOCKET_ERROR)
	{
		printf("Can't bind [%d]\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	// 에코
	printf("Receiving...\n");
	char message[BUFFER_SIZE] = { 0 };
	sockaddr clientAddress = { 0 };
	int sz = 0;
	int receivedByte = recvfrom(serverSocket, message, BUFFER_SIZE, NULL, &clientAddress, &sz);
	if (receivedByte < 0)
	{
		printf("Can't receive the message from the client [%d]\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	shutdown(serverSocket, SD_RECEIVE);

	result = sendto(serverSocket, message, receivedByte, NULL, &clientAddress, sz);
	if (result == SOCKET_ERROR)
	{
		printf("Can't send to the client [%d]\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	printf("Echoed : %s\n", message);

	// 연결 해제
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}