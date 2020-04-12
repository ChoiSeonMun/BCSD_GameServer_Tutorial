#include <iostream>
#include <WinSock2.h>
#include <tchar.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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

int _tmain(int argc, WCHAR* argv[])
{
	// 명령행 인자 검사
	if (argc != 4)
	{
		wprintf(L"[!] Used : ./PortScanner.exe [IPv4 Address] [Port Minimum] [Port Maximum]\n");
		return 1;
	}

	// Winsock 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		PrintErrorWithMessage(L"winsock lib can't be loaded", GetLastError());
		return 1;
	}

	// 명령행 인자 파싱
	WCHAR* addr = argv[1];
	WORD portMin;
	WORD portMax;
	swscanf_s(argv[2], L"%hd", &portMin);
	swscanf_s(argv[3], L"%hd", &portMax);

	// 명령행 인자 예외 처리
	DWORD remoteAddr;
	if (InetPton(AF_INET, addr, &remoteAddr) != 1)
	{
		PrintErrorWithMessage(L"Please enter the right IPv4 Address", WSAGetLastError());
		return 1;
	}

	SOCKET s = INVALID_SOCKET;
	do
	{
		// 포트 스캐닝
		DWORD count = 0;
		for (size_t port = portMin; port <= portMax; port++)
		{
			SOCKADDR_IN in;
			in.sin_family = AF_INET;
			in.sin_addr.s_addr = remoteAddr;
			in.sin_port = htons(port);

			s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s == INVALID_SOCKET)
			{
				PrintErrorWithMessage(L"socket can't be created.", WSAGetLastError());
				break;
			}

			if (connect(s, (const sockaddr*)&in, sizeof(in)) == NULL)
			{
				++count;
			}

			closesocket(s);
			s = INVALID_SOCKET;
		}

		wprintf_s(L"The count of listening socket : %d\n", count);
	} while (false);
	
	closesocket(s);
	WSACleanup();
}