#pragma once

#include "pch.h"

#define DEFAULT_PORT 27015
#define BUFFER_SIZE 512

#pragma comment (lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <cstdlib>

