#include "Common.h"

int _tmain()
{
	setlocale(LC_ALL, "KOREAN");
	wprintf(L"Server start...!\n");

	// Initialize winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		PrintErrorWithMessage(L"winsock couldn't be initialized.", GetLastError());
		return 1;
	}

	SOCKET listener = INVALID_SOCKET;
	SOCKET client = INVALID_SOCKET;
	do
	{
		// Create a socket
		listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listener == INVALID_SOCKET)
		{
			PrintErrorWithMessage(L"listen socket couldn't be created.", WSAGetLastError());
			break;
		}

		// Bind
		SOCKADDR_IN in;
		in.sin_family = AF_INET;
		in.sin_port = htons(PORT_ECHO_SERVER);
		in.sin_addr.s_addr = INADDR_ANY;

		if (bind(listener, (const sockaddr*)&in, sizeof(in)) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"socket couldn't be bound.", WSAGetLastError());
			break;
		}

		// Listen
		if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"socket couldn't listen.", WSAGetLastError());
			break;
		}
		wprintf(L"Waiting for clients...\n");

		// Accept
		SOCKADDR_IN clientIn;
		int clientInSize = sizeof(clientIn);
		client = accept(listener, (sockaddr*)&clientIn, &clientInSize);
		if (client == INVALID_SOCKET)
		{
			PrintErrorWithMessage(L"client can't be accepted.", WSAGetLastError());
			break;
		}

		WCHAR buffer[1024];
		buffer[0] = '\0';
		InetNtop(AF_INET, &clientIn.sin_addr, buffer, 1024);
		wprintf(L"Client accepted : [%s:%d]\n", buffer, ntohs(clientIn.sin_port));

		// Echo
		buffer[0] = '\0';
		WCHAR* message = buffer;
		int messageLength = sizeof(buffer);
		int recvByte = 0;
		bool hasReceived = false;
		do
		{
			recvByte = recv(client, (char*)message, messageLength, NULL);
			if (recvByte == 0)
			{
				wprintf(L"Closed...\n");
			}
			else if (recvByte == SOCKET_ERROR)
			{
				PrintErrorWithMessage(L"can't receive from the client", WSAGetLastError());
			}
			else
			{
				message += recvByte / sizeof(WCHAR);
				messageLength -= recvByte;
				hasReceived = true;
			}
		} while (recvByte > 0);

		shutdown(client, SD_RECEIVE);
		if (hasReceived)
		{
			wprintf(L"Received : %s\n", buffer);

			// send
			if (send(client, (const char*)buffer, sizeof(buffer), NULL) == SOCKET_ERROR)
			{
				PrintErrorWithMessage(L"can't send the message to the client",
					WSAGetLastError());
				break;
			}
			shutdown(client, SD_SEND);
			wprintf(L"Echoed : %s\n", buffer);
		}
	} while (false);

	// Cleanup
	closesocket(listener);
	closesocket(client);
	WSACleanup();
}