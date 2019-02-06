// ��� ����������������� ����������:

#include "stdafx.h"
#include <cstring>
#include "UserFunc.h"

void inputWord(char* const word, int len)
{
	do
	{
		printf("������� �����>");
		fgets(word, len, stdin);
		word[strcspn(word, "\n")] = '\0';
		printf("�� ����� �����: '%s'\n", word);
	} while (!checkWord(word));

	
}

// �������� ����� �� ������������:
bool checkWord(const char* word)
{
	bool hasEnglish = false, hasRussian = false;

	int wordLen = strlen(word);
	for (int i = 0; i < wordLen; ++i)
	{
		if (word[i] >= '0' && word[i] <= '9') 
		{
			printf("������! � ����� ������������ �����!\n");
			return false;
		}
		else if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
		{
			hasEnglish = true;
		}
		else if ((word[i] >= '�' && word[i] <= '�') || (word[i] >= '�' && word[i] <= '�'))
		{
			hasRussian = true;
		}

		switch (word[i])
		{
		case ',': case '.': case '!': case '?': case ';': case ':':
			printf("������! � ����� ������������ ����� ����������!\n");
			return false;
		default:
			break;
		}

		if (hasEnglish && hasRussian)
		{
			printf("������! �������� ��������� � ������������� ��������!\n");
			return false;
		}
	}
	return true;
}
