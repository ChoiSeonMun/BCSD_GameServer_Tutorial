#include "Common.h"


int _tmain()
{
	setlocale(LC_ALL, "KOREAN");
	wprintf(L"Client Start...\n");

	// winsock intialize
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		PrintErrorWithMessage(L"winsock can't be initialized", GetLastError());
		return 1;
	}

	SOCKET s = INVALID_SOCKET;
	do
	{
		// create a socket
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		wprintf(L"Connect to the server...\n");

		// connect to the server
		SOCKADDR_IN in;
		in.sin_family = AF_INET;
		in.sin_port = htons(PORT_ECHO_SERVER);
		InetPton(AF_INET, L"127.0.0.1", &in.sin_addr);
		if (connect(s, (const sockaddr*)&in, sizeof(in)) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"can't connect to the server", WSAGetLastError());
			break;
		}

		// send to the server
		WCHAR message[1024];
		message[0] = '\0';
		wprintf(L"Enter the message : ");
		_getws_s(message);
		if (send(s, (const char*)message, sizeof(message), NULL) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"can't send to the server", WSAGetLastError());
			break;
		}
		wprintf(L"Message Sent : [%s]\n", message);
		shutdown(s, SD_SEND);

		// receive
		int recvByte = 0;
		WCHAR* buffer = message;
		int bufferLen = sizeof(message);
		bool hasReceived = false;
		do
		{
			recvByte = recv(s, (char*)buffer, bufferLen, NULL);
			if (recvByte == 0)
			{
				wprintf(L"Closed...\n");
			}
			else if (recvByte == SOCKET_ERROR)
			{
				PrintErrorWithMessage(L"can't receive the message from the server",
					WSAGetLastError());
			}
			else
			{
				buffer += recvByte / 2;
				bufferLen -= recvByte;
				hasReceived = true;
			}
		} while (recvByte > 0);
		
		if (hasReceived)
		{
			wprintf(L"Received : %s\n", message);
		}
		shutdown(s, SD_RECEIVE);
	} while (false);

	// Clean up
	closesocket(s);
	WSACleanup();
}