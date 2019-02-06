// Код пользовательского интерфейса:

#include "stdafx.h"
#include <cstring>
#include "UserFunc.h"

void inputWord(char* const word, int len)
{
	do
	{
		printf("Введите слово>");
		fgets(word, len, stdin);
		word[strcspn(word, "\n")] = '\0';
		printf("Вы ввели слово: '%s'\n", word);
	} while (!checkWord(word));

	
}

// Проверка слова на корректность:
bool checkWord(const char* word)
{
	bool hasEnglish = false, hasRussian = false;

	int wordLen = strlen(word);
	for (int i = 0; i < wordLen; ++i)
	{
		if (word[i] >= '0' && word[i] <= '9') 
		{
			printf("Ошибка! В слове присутствуют цифры!\n");
			return false;
		}
		else if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
		{
			hasEnglish = true;
		}
		else if ((word[i] >= 'а' && word[i] <= 'я') || (word[i] >= 'А' && word[i] <= 'Я'))
		{
			hasRussian = true;
		}

		switch (word[i])
		{
		case ',': case '.': case '!': case '?': case ';': case ':':
			printf("Ошибка! В слове присутствуют знаки препинания!\n");
			return false;
		default:
			break;
		}

		if (hasEnglish && hasRussian)
		{
			printf("Ошибка! Смешение латинских и кириллических символов!\n");
			return false;
		}
	}
	return true;
}
