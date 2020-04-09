#pragma once

#define WIN32_LEAN_AND_MEAN

#define PORT_ECHO_SERVER	10097

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <cstdint>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void PrintErrorWithMessage(LPCWSTR message, DWORD errorCode)
{
	WCHAR buffer[1024];
	buffer[0] = '\0';
	DWORD englishLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, englishLocale,
		buffer, 1024, NULL
		);

	fwprintf_s(stderr, L"[!] %s\n", message);
	fwprintf_s(stderr, L"[Error] %s\n", buffer);
}