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
	LOG(L"Client Start!");

	SOCKET client = INVALID_SOCKET;
	do
	{
		// 소켓 생성
		client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (client == INVALID_SOCKET)
		{
			PrintErrorWithMessage(L"socket()", WSAGetLastError());
			break;
		}

		// 연결
		SOCKADDR_IN remote = { 0 };
		remote.sin_family = AF_INET;
		remote.sin_port = htons(PORT_TCP_ECHO);
		remote.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		if (connect(client, (const sockaddr*)&remote, sizeof(remote)) == SOCKET_ERROR)
		{
			PrintErrorWithMessage(L"connect()", WSAGetLastError());
			break;
		}
		LOG(L"Connection Complete!");

		while (true)
		{
			// 데이터 송수신; 패킷의 길이는 고정 길이다.
			wprintf(L"Input message : ");
			WCHAR msg[BUFFER_SIZE];
			_getws_s(msg);
			if (send(client, (const char*)msg, sizeof(msg), 0) == SOCKET_ERROR)
			{
				PrintErrorWithMessage(L"send()", WSAGetLastError());
				break;
			}
			FORMATTED_LOG(L"Message Sent");

			int recvByte = recvn(client, msg, _countof(msg), 0);
			if (recvByte < 0)
			{
				PrintErrorWithMessage(L"recvn()", WSAGetLastError());
				break;
			}
			else if (recvByte == 0)
			{
				LOG(L"Connection Closed");
			}
			else
			{
				wprintf(L"[Echo] %s\n", msg);
			}
		}
	} while (false);

	// 정리
	closesocket(client);
	WSACleanup();

	return NO_ERROR;
}