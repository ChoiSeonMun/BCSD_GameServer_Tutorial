#include "stdafx.h"

int wmain()
{
	// Winsock2 초기화
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		PrintErrorWithMessage(L"WSAStartup()", WSAGetLastError());
		return 1;
	}
	LOG(L"Server start");

	SOCKET listener = INVALID_SOCKET;
	SOCKET client = INVALID_SOCKET;
	do
	{
		// 소켓 생성
		listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listener == INVALID_SOCKET)
		{
			PrintErrorWithMessage(L"socket()", WSAGetLastError());
			break;
		}

		// 바인드
		SOCKADDR_IN in;
		in.sin_family = AF_INET;
		in.sin_port = htons(PORT_TCP_ECHO);
		in.sin_addr.s_addr = INADDR_ANY;
		if (bind(listener, (const sockaddr*)&in, sizeof(in)) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"bind()", WSAGetLastError());
			break;
		}

		// 대기
		if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"listen()", WSAGetLastError());
			break;
		}
		FORMATTED_LOG(L"Listening using %d", PORT_TCP_ECHO);

		while (true)
		{
			// 연결 수용
			SOCKADDR_IN clientIn;
			int			clientInSize = sizeof(clientIn);
			client = accept(listener, (sockaddr*)&clientIn, &clientInSize);
			if (client == INVALID_SOCKET)
			{
				PrintErrorWithMessage(L"accept()", WSAGetLastError());
				break;
			}
			WCHAR buffer[32];
			InetNtop(AF_INET, &(clientIn.sin_addr.s_addr), buffer, _countof(buffer));
			FORMATTED_LOG(L"[%s:%d] Connected", buffer, ntohs(clientIn.sin_port));

			while (true)
			{
				// 데이터 송수신
				WCHAR msg[BUFFER_SIZE];
				int recvByte = recvn(client, msg, _countof(msg), 0);
				if (recvByte < 0)
				{
					PrintErrorWithMessage(L"recvn()", WSAGetLastError());
					break;
				}
				else if (recvByte == 0)
				{
					FORMATTED_LOG(L"[%s:%d] Connection Closed", buffer, ntohs(clientIn.sin_port));
					break;
				}
				else
				{
					wprintf(L"[%s:%d] %s\n", buffer, ntohs(clientIn.sin_port), msg);
					if (send(client, (const char*)msg, sizeof(msg), 0) == SOCKET_ERROR)
					{
						PrintErrorWithMessage(L"send()", WSAGetLastError());
						break;
					}
					LOG(L"Echo Complete!");
				}
			}
		}
	} while (false);

	// 정리
	closesocket(listener);
	closesocket(client);
	WSACleanup();

	return NO_ERROR;
}