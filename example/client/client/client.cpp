// client.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <clocale>
#include <string>
#include "func.h"

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	cout << "¬ведите слово: ";
	string s;
	cin >> s;

	CheckPalindrom(s);
	
	_getch();
    return 0;
}

