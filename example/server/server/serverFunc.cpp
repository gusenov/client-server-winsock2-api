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
	const int waitingTime = 2000; // ����� �������� � 2000 �����������

	SingleNamedPipe = CreateNamedPipe(L"\\\\.\\Pipe\\pipe1"
		, PIPE_ACCESS_DUPLEX
		, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
		, theOnlyInstance, 0, 0, waitingTime
		, NULL
	);
	if (INVALID_HANDLE_VALUE == SingleNamedPipe)
	{
		printf("������ ��� �������� ��������� %d.\n", GetLastError());
		_getch();
		return;
	}

	printf("������ ������� � ���� ����������� �������.\n");
	if (0 == ConnectNamedPipe(SingleNamedPipe, NULL))
	{
		printf("������ ��� ������������ ���������� %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	char buffer[BUFFER_SIZE]; // ����� ��� ������ �� ���������
	DWORD BytesRead; // ���������� ��������� ������
	if (0 >= ReadFile(SingleNamedPipe, buffer, sizeof(buffer), &BytesRead, NULL))
	{
		printf("������ ��� ������ �� ��������� %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	buffer[BytesRead] = 0; // ��������� ���������� ����� � ������
	printf("%s\n", buffer);

	DWORD BytesWritten;

	string s(buffer);
	string msg = (equal(s.begin(), s.end(), s.rbegin())) ? "����� �������� �����������" : "����� �� �������� �����������";
	if (0 == WriteFile(SingleNamedPipe, msg.c_str(), strlen(msg.c_str()), &BytesWritten, NULL))
	{
		printf("������ � �������� � ������� %d.\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}

	// �� ��������� ������ ����� ������� ��� ���������� � ����������
	if (0 == DisconnectNamedPipe(SingleNamedPipe))
	{
		printf("������ ��� ������� ���������� %d\n", GetLastError());
		_getch();
		CloseHandle(SingleNamedPipe);
		return;
	}
	
	CloseHandle(SingleNamedPipe);
}