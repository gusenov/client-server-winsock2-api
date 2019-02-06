#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "func.h"

using namespace std;


void CheckPalindrom(string s)
{
	DWORD BytesWritten;

	string letters = "абвгдеЄжзийклмнопрстуфхцчшщъыьэю€јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёя";

	// ожидание своей очереди подключени€ к конвейеру на сервере
	// предполагаетс€, что сервер и клиент запущены на одной машине
	if (0 == WaitNamedPipe(L"\\\\.\\Pipe\\pipe1", NMPWAIT_WAIT_FOREVER))
	{
		printf("ќжидание соединени€ с ошибкой %d.\n", GetLastError());
		_getch();
		return;
	}

	// создание описател€ существующего » 
	HANDLE ServerPipe;
	ServerPipe = CreateFile(L"\\\\.\\Pipe\\pipe1", GENERIC_READ | GENERIC_WRITE, 0
		, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL
	);
	if (INVALID_HANDLE_VALUE == ServerPipe)
	{
		printf("ќткрытие конвейера с ошибкой %d.\n", GetLastError());
		_getch();
		return;
	}

	size_t found = s.find_first_not_of(letters);
	if (found != string::npos) {
		cout << "¬ слове присутствуют недопустимые символы!" << endl;
		_getch();
		CloseHandle(ServerPipe);
		return;
	}

	if (0 == WriteFile(ServerPipe, s.c_str(), strlen(s.c_str()), &BytesWritten, NULL))
	{
		printf("«апись в конвейер с ошибкой %d.\n", GetLastError());
		_getch();
		CloseHandle(ServerPipe);
		return;
	}

	char buffer[256]; // буфер дл€ чтени€ из конвейера
	DWORD BytesRead; // количество считанных байтов
	if (0 >= ReadFile(ServerPipe, buffer, sizeof(buffer), &BytesRead, NULL))
	{
		printf("ќшибка при чтении из конвейера %d.\n", GetLastError());
		_getch();
		CloseHandle(ServerPipe);
		return;
	}

	buffer[BytesRead] = 0; // обнуление последнего байта в буфере
	printf("%s\n", buffer);

	CloseHandle(ServerPipe);
	return;
}