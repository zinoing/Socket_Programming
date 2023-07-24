// conv_addr_win.c : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) {
	const char* strAddr = "203.211.218.102:9190";

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	size = sizeof(servAddr);
	WSAStringToAddress(
		(LPSTR)strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size
	);

	size = sizeof(strAddrBuf);
	WSAAddressToString(
		(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, (LPSTR)strAddrBuf, (LPDWORD)size
	);

	printf("Second conv result: %s \n", strAddrBuf);
	WSACleanup();
	return 0;
}
