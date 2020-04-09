#pragma once

#define DEFAULT_TCP_PORT 27015
#define DEFAULT_UDP_PORT 12345
#define BUFFER_SIZE 512

#pragma comment (lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <cstdlib>
#include <tchar.h>