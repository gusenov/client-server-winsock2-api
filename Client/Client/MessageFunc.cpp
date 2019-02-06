// Код, реализующий операции по обмену сообщениями:

#include "stdafx.h"
#include "MessageFunc.h"

long loadWinSockLibrary()
{
	WORD libraryVersion = 0x0202;  // номер версии.

	WSADATA wsaData;

	// Загрузка нужной версии библиотеки WinSock:
	long errorCode = WSAStartup(
		  libraryVersion  // номер версии.
		, &wsaData  // структура с информацией о версии.
	);

	// Обрабатываем состояние ошибки, которое возвращает функция WSAStartup:
	if (errorCode == 0)
	{
		printf("Библиотека инициализирована.\n");
	}
	else
	{
		printf("Невозможно загрузить Winsock 2.2!\n");
		printErrorMessage(errorCode);
	}
	return errorCode;
}

long unloadWinSockLibrary()
{
	WSACleanup();  // выгрузка библиотеки сокетов.
	long errorCode = WSAGetLastError();
	if (errorCode != 0)
	{
		printErrorMessage(errorCode);
	}
	return errorCode;
}

long createDatagramSocket(SOCKET* socket)
{
	// Сокет создается функцией WSASocket:
	*socket = WSASocket(
		  AF_INET  // семейство адресов протокола.
		, SOCK_DGRAM  // тип сокета для данного протокола.
		, IPPROTO_UDP  // транспорт.
		, NULL  // информация о протоколе.
		, 0  // зарезервировано для будущего использования.
		, 0  // дополнительные возможности сокета.
	);
	
	// Проверяем, удалось ли создать сокет:
	if (INVALID_SOCKET == *socket)
	{
		long errorCode = WSAGetLastError();
		printf("Ошибка создания сокета с кодом = %d!\n", errorCode);
		printErrorMessage(errorCode);

		// Завершаем выполнение в случае ошибки:
		return errorCode;
	}

	return 0;
}

void closeSocket(SOCKET socket)
{
	closesocket(socket);
}

// Привязка сокета протокола к его имени:
long bindSocket(SOCKET unboundSocket, INT port)
{
	// Структура с адресом и номером порта:
	SOCKADDR_IN networkAddress;

	// для IP должен использоваться AF_INET:
	networkAddress.sin_family = AF_INET;
	
	// Позволяет серверу слушать клиента 
	// через любой сетевой интерфейс на несущем компьютере:
	networkAddress.sin_addr.s_addr = INADDR_ANY;
	
	// Преобразование порта (короткого целого числа) 
	// из системного порядка байтов в сетевой:
	networkAddress.sin_port = htons(port);

	// Привязка сокета к интерфейсу, на котором будут
	// приниматься данные, с помощью функции bind:
	int bindingResult = bind(
		  unboundSocket  // сокет, ожидающий соединения клиентов.
		, (struct sockaddr*)&networkAddress  // буфер со структурой, содержащей адрес.
		, sizeof(networkAddress)  // длина буфера.
	);

	if (SOCKET_ERROR == bindingResult)
	{
		int errorCode = WSAGetLastError();
		printf("Ошибка bind() с кодом = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}

	return bindingResult;
}

// Приём данных по сокету:
long receiveMessage(SOCKET socket, char* buffer, ULONG bufferSize, char** senderIp)
{
	printf("Слушаю и принимаю дейтаграммы.\n");

	WSABUF dataBuffer;
	dataBuffer.len = bufferSize;
	dataBuffer.buf = buffer;

	SOCKADDR_IN senderAddr;
	int senderAddrSize = sizeof (senderAddr);

	DWORD recievedBytes;
	DWORD dwFlags = 0;
	
	int recvResult = WSARecvFrom(
		  socket  // сокет для приема данных.
		, &dataBuffer  // буферы с данными для приема.
		, 1  // количество буферов для приема.
		, &recievedBytes  // количество принятых байтов.
		, &dwFlags  // дополнительные флаги.
		, (SOCKADDR *)&senderAddr  // адрес станции-отправителя.
		, &senderAddrSize  // длина адреса отправителя.
		, NULL  // структура для перекрытого ввода-вывода.
		, NULL  // процедура для завершения перекрытого ввода-вывода.
	);
	
	if (SOCKET_ERROR == recvResult)
	{
		long errorCode = WSAGetLastError();
		printf("Ошибка WSARecvFrom() с кодом = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}
	else
	{
		*senderIp = inet_ntoa(senderAddr.sin_addr);
		printf("От %s принято сообщение '%s' из %d байтов.\n"
			, *senderIp
			, dataBuffer.buf
			, recievedBytes
		);
	}

	return recvResult;
}

// Отправка данных:
long sendMessage(SOCKET socket, char* message, const char* ip, INT port)
{
	WSABUF dataBuffer;
	dataBuffer.len = strlen(message) + 1;
	dataBuffer.buf = message;
	
	SOCKADDR_IN receiver;
	receiver.sin_family = AF_INET;
	receiver.sin_addr.s_addr = inet_addr(ip);
	receiver.sin_port = htons(port);

	DWORD sendBytes;

	int sendResult = WSASendTo(
		  socket  // сокет для отправки данных.
		, &dataBuffer  // буферы для отправляемых данных.
		, 1  // количество буферов для отправки.
		, &sendBytes  // число отправляемых байтов.
		, 0  // дополнительные флаги.
		, (SOCKADDR*)&receiver  // адрес станции-приемника.
		, sizeof (receiver)  // длина адреса приемника.
		, NULL  // структура для перекрытого ввода-вывода.
		, NULL  // процедура для завершения перекрытого ввода-вывода.
	);
	
	if (SOCKET_ERROR == sendResult)
	{
		long errorCode = WSAGetLastError();
		printf("Ошибка WSASendTo() с кодом = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}
	
	printf("Послано %d байтов.\n", sendBytes);
	return sendResult;
}

void printErrorMessage(long errorCode)
{
	switch (errorCode)
	{
	case WSASYSNOTREADY:
		printf("Отсутствие готовности для работы в сетевой среде!\n");
		break;

	case WSAVERNOTSUPPORTED:
		printf("Отсутствие поддержки заданной версии библиотеки сокетов!\n");
		break;

	case WSAEINPROGRESS:
		printf("Незавершенная блокирующая операция библиотеки сокетов версии 1.1!\n");
		break;

	case WSAEPROCLIM:
		printf("Достигнуто ограничение на количество заданий в заданной версии библиотеки сокетов!\n");
		break;

	case WSAEFAULT:
		printf("Переданы неверный параметр lpWSAData!\n");
		break;
	
	case WSANOTINITIALISED:
		printf("Ещё не вызывалась функция WSAStartup!\n");
		break;

	case WSAENETDOWN:
		printf("Сбой сети!\n");
		break;
	}
}