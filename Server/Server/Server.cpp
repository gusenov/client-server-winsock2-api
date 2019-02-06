// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MessageFunc.h"
#include <string>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	loadWinSockLibrary();
	SOCKET serverSocket, clientSocket;
	createDatagramSocket(&serverSocket);
	
	printf("Введите для сервера любой свободный порт из диапазона (1024, 65535)>");
	int myPort;
	cin >> myPort;
	bindSocket(serverSocket, myPort);

	createDatagramSocket(&clientSocket);
	
	char buffer[RECEIVE_BUFFER_DEFAULT_MAX_LEN] = "\0";
	char* senderIp = 0;

	printf("Введите номер порта на который сервер будет отвечать клиентам>");
	int senderPort;
	cin >> senderPort;
	getchar();

	while (true)
	{
		receiveMessage(serverSocket, buffer, sizeof (buffer), &senderIp);
		string word(buffer);
		bool result = equal(word.begin(), word.end(), word.rbegin());

		sendMessage(clientSocket, result ? "Слово является палиндромом." 
			: "Слово не является палиндромом.", senderIp, senderPort);
	}

	closeSocket(serverSocket);
	closeSocket(clientSocket);
	unloadWinSockLibrary();

	getchar();
	return 0;
}

