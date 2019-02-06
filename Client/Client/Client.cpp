// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UserFunc.h"
#include "MessageFunc.h"
#include <iostream>

// ������������ ����� �����:
#define WORD_MAX_LEN 1000

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	loadWinSockLibrary();
	SOCKET serverSocket, clientSocket;
	createDatagramSocket(&serverSocket);
	
	printf("������� ��� ������� ����� ��������� ���� �� ��������� (1024, 65535)>");
	int myPort;
	cin >> myPort;
	getchar();
	bindSocket(serverSocket, myPort);

	createDatagramSocket(&clientSocket);

	char word[WORD_MAX_LEN];

	char buffer[RECEIVE_BUFFER_DEFAULT_MAX_LEN] = "\0";
	char* senderIp = 0;
	int senderPort;
	
	printf("������� IP-����� �������>");
	char serverIp[16];
	fgets(serverIp, sizeof serverIp, stdin);
	serverIp[strcspn(serverIp, "\n")] = '\0';

	printf("������� ����� ����� �������>");
	int serverPort;
	cin >> serverPort;
	getchar();

	while (true)
	{
		// ������ ��������� �� ������������ ����� �����:
		inputWord(word, sizeof word);

		// � �������� ��� �������:
		sendMessage(clientSocket, word, serverIp, serverPort);

		// ������ ��������� �����, ���������, �������� �� ��������� ����� �����������, 
		// � ����������� ������������ � ���������� ��������:
		receiveMessage(serverSocket, buffer, sizeof (buffer), &senderIp);

		printf("��������� ��������:\n%s\n", buffer);
	}

	closeSocket(serverSocket);
	closeSocket(clientSocket);
	unloadWinSockLibrary();

	system("PAUSE");
	return 0;
}

