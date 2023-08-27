// oob_recv_win.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 30
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hAcptSock, hRecvSock;
	SOCKADDR_IN recvAdr;
	SOCKADDR_IN sendAdr;
	int sendAdrSize, strLen;
	char buf[BUF_SIZE];
	int result;

	fd_set read, except, readCopy, exceptCopy;
	struct timeval timeout;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hAcptSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hAcptSock, (SOCKADDR*)&recvAdr, sizeof(recvAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	if (listen(hAcptSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	sendAdrSize = sizeof(sendAdr);
	hRecvSock = accept(hRecvSock, (SOCKADDR*)&sendAdr, &sendAdrSize);

	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(hRecvSock, &read);
	FD_SET(hRecvSock, &except);

	while (1) {
		readCopy = read;
		exceptCopy = except;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);

		if (result > 0)
		{
			if (FD_ISSET(hRecvSock, &exceptCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				printf("Urgent message: %s \n", buf);
			}

			if (FD_ISSET(hRecvSock, &readCopy))
			{
				strLen = recv(hRecvSock, buf, BUF_SIZE - 1, 0);
				if (strLen == 0)
				{
					break;
					closesocket(hRecvSock);
				}
				else
				{
					buf[strLen] = 0;
					puts(buf);
				}
			}
		}
	}

	closesocket(hAcptSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
