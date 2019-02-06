// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UserFunc.h"
#include "MessageFunc.h"
#include <iostream>

// Максимальная длина слова:
#define WORD_MAX_LEN 1000

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	loadWinSockLibrary();
	SOCKET serverSocket, clientSocket;
	createDatagramSocket(&serverSocket);
	
	printf("Введите для клиента любой свободный порт из диапазона (1024, 65535)>");
	int myPort;
	cin >> myPort;
	getchar();
	bindSocket(serverSocket, myPort);

	createDatagramSocket(&clientSocket);

	char word[WORD_MAX_LEN];

	char buffer[RECEIVE_BUFFER_DEFAULT_MAX_LEN] = "\0";
	char* senderIp = 0;
	int senderPort;
	
	printf("Введите IP-адрес сервера>");
	char serverIp[16];
	fgets(serverIp, sizeof serverIp, stdin);
	serverIp[strcspn(serverIp, "\n")] = '\0';

	printf("Введите номер порта сервера>");
	int serverPort;
	cin >> serverPort;
	getchar();

	while (true)
	{
		// Клиент принимает от пользователя некое слово:
		inputWord(word, sizeof word);

		// И отсылает его серверу:
		sendMessage(clientSocket, word, serverIp, serverPort);

		// Сервер принимает слово, проверяет, является ли введенное слово палиндромом, 
		// и информирует пользователя о результате проверки:
		receiveMessage(serverSocket, buffer, sizeof (buffer), &senderIp);

		printf("Результат проверки:\n%s\n", buffer);
	}

	closeSocket(serverSocket);
	closeSocket(clientSocket);
	unloadWinSockLibrary();

	system("PAUSE");
	return 0;
}

