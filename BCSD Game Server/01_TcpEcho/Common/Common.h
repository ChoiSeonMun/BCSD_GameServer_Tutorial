#pragma once

#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <tchar.h>

using namespace std;

#define PORT_TCP_ECHO	27514
#define BUFFER_SIZE		1024

#define FORMATTED_LOG(format, ...)	wprintf(L"[Log] "); wprintf(format, __VA_ARGS__); wprintf(L"\n")
#define LOG(msg)					wprintf(L"[Log] %s\n", msg)

VOID PrintErrorWithMessage(LPCWSTR functionName, DWORD errorCode)
{
	static DWORD englishLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	static WCHAR buffer[1024];
	
	buffer[0] = '\0';
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, englishLocale,
		buffer, 1024, NULL
	);

	fwprintf_s(stderr, L"[Error at %s] %s\n", functionName, buffer);
}

int recvn(SOCKET s, WCHAR* buffer, int len, int flags)
{
	char* p = (char*)buffer;
	int leftLen = len * 2;
	int recvByte = 0;
	do
	{
		recvByte = recv(s, p, leftLen, flags);
		if (recvByte > 0)
		{
			p += recvByte;
			leftLen -= recvByte;
		}
		else if (recvByte == 0)
		{
			return 0;
		}
		else
		{
			return SOCKET_ERROR;
		}
	} while (leftLen > 0);

	return len - leftLen;
}