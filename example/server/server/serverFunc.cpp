#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <clocale>
#include <iostream>
#include <string>
#include <conio.h>
#include "serverFunc.h"

#define BUFFER_SIZE 256

using namespace std;

void GetAndCheckPalindrom()
{
	HANDLE SingleNamedPipe;
	const int theOnlyInstance = 1;
	const int waitingTime = 2000; // время ожидания – 2000 миллисекунд

	SingleNamedPipe = CreateNamedPipe(L"\\\\.\\Pipe\\pipe1"
		, PIPE_ACCESS_DUPLEX
		, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
		, theOnlyInstance, 0, 0, waitingTime
		, NULL
	);
	if (INVALID_HANDLE_VALUE == SingleNamedPipe)
	{
		printf("Ошибка при создании конвейера %d.\n", GetLastError());
		_getch();
		return;
	}

	printf("Сервер запущен и ждет подключения клиента.\n");
	if (0 == ConnectNamedPipe(SingleNamedPipe, NULL))
	{
		printf("Ошибка при установлении соединения %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	char buffer[BUFFER_SIZE]; // буфер для чтения из конвейера
	DWORD BytesRead; // количество считанных байтов
	if (0 >= ReadFile(SingleNamedPipe, buffer, sizeof(buffer), &BytesRead, NULL))
	{
		printf("Ошибка при чтении из конвейера %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	buffer[BytesRead] = 0; // обнуление последнего байта в буфере
	printf("%s\n", buffer);

	DWORD BytesWritten;

	string s(buffer);
	string msg = (equal(s.begin(), s.end(), s.rbegin())) ? "Слово является палиндромом" : "Слово не является палиндромом";
	if (0 == WriteFile(SingleNamedPipe, msg.c_str(), strlen(msg.c_str()), &BytesWritten, NULL))
	{
		printf("Запись в конвейер с ошибкой %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	// по окончании чтения нужно закрыть все соединения с конвейером
	if (0 == DisconnectNamedPipe(SingleNamedPipe))
	{
		printf("Ошибка при разрыве соединения %d\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}
	
	CloseHandle(SingleNamedPipe);
}