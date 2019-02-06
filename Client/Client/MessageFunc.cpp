// ���, ����������� �������� �� ������ �����������:

#include "stdafx.h"
#include "MessageFunc.h"

long loadWinSockLibrary()
{
	WORD libraryVersion = 0x0202;  // ����� ������.

	WSADATA wsaData;

	// �������� ������ ������ ���������� WinSock:
	long errorCode = WSAStartup(
		  libraryVersion  // ����� ������.
		, &wsaData  // ��������� � ����������� � ������.
	);

	// ������������ ��������� ������, ������� ���������� ������� WSAStartup:
	if (errorCode == 0)
	{
		printf("���������� ����������������.\n");
	}
	else
	{
		printf("���������� ��������� Winsock 2.2!\n");
		printErrorMessage(errorCode);
	}
	return errorCode;
}

long unloadWinSockLibrary()
{
	WSACleanup();  // �������� ���������� �������.
	long errorCode = WSAGetLastError();
	if (errorCode != 0)
	{
		printErrorMessage(errorCode);
	}
	return errorCode;
}

long createDatagramSocket(SOCKET* socket)
{
	// ����� ��������� �������� WSASocket:
	*socket = WSASocket(
		  AF_INET  // ��������� ������� ���������.
		, SOCK_DGRAM  // ��� ������ ��� ������� ���������.
		, IPPROTO_UDP  // ���������.
		, NULL  // ���������� � ���������.
		, 0  // ��������������� ��� �������� �������������.
		, 0  // �������������� ����������� ������.
	);
	
	// ���������, ������� �� ������� �����:
	if (INVALID_SOCKET == *socket)
	{
		long errorCode = WSAGetLastError();
		printf("������ �������� ������ � ����� = %d!\n", errorCode);
		printErrorMessage(errorCode);

		// ��������� ���������� � ������ ������:
		return errorCode;
	}

	return 0;
}

void closeSocket(SOCKET socket)
{
	closesocket(socket);
}

// �������� ������ ��������� � ��� �����:
long bindSocket(SOCKET unboundSocket, INT port)
{
	// ��������� � ������� � ������� �����:
	SOCKADDR_IN networkAddress;

	// ��� IP ������ �������������� AF_INET:
	networkAddress.sin_family = AF_INET;
	
	// ��������� ������� ������� ������� 
	// ����� ����� ������� ��������� �� ������� ����������:
	networkAddress.sin_addr.s_addr = INADDR_ANY;
	
	// �������������� ����� (��������� ������ �����) 
	// �� ���������� ������� ������ � �������:
	networkAddress.sin_port = htons(port);

	// �������� ������ � ����������, �� ������� �����
	// ����������� ������, � ������� ������� bind:
	int bindingResult = bind(
		  unboundSocket  // �����, ��������� ���������� ��������.
		, (struct sockaddr*)&networkAddress  // ����� �� ����������, ���������� �����.
		, sizeof(networkAddress)  // ����� ������.
	);

	if (SOCKET_ERROR == bindingResult)
	{
		int errorCode = WSAGetLastError();
		printf("������ bind() � ����� = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}

	return bindingResult;
}

// ���� ������ �� ������:
long receiveMessage(SOCKET socket, char* buffer, ULONG bufferSize, char** senderIp)
{
	printf("������ � �������� �����������.\n");

	WSABUF dataBuffer;
	dataBuffer.len = bufferSize;
	dataBuffer.buf = buffer;

	SOCKADDR_IN senderAddr;
	int senderAddrSize = sizeof (senderAddr);

	DWORD recievedBytes;
	DWORD dwFlags = 0;
	
	int recvResult = WSARecvFrom(
		  socket  // ����� ��� ������ ������.
		, &dataBuffer  // ������ � ������� ��� ������.
		, 1  // ���������� ������� ��� ������.
		, &recievedBytes  // ���������� �������� ������.
		, &dwFlags  // �������������� �����.
		, (SOCKADDR *)&senderAddr  // ����� �������-�����������.
		, &senderAddrSize  // ����� ������ �����������.
		, NULL  // ��������� ��� ����������� �����-������.
		, NULL  // ��������� ��� ���������� ����������� �����-������.
	);
	
	if (SOCKET_ERROR == recvResult)
	{
		long errorCode = WSAGetLastError();
		printf("������ WSARecvFrom() � ����� = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}
	else
	{
		*senderIp = inet_ntoa(senderAddr.sin_addr);
		printf("�� %s ������� ��������� '%s' �� %d ������.\n"
			, *senderIp
			, dataBuffer.buf
			, recievedBytes
		);
	}

	return recvResult;
}

// �������� ������:
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
		  socket  // ����� ��� �������� ������.
		, &dataBuffer  // ������ ��� ������������ ������.
		, 1  // ���������� ������� ��� ��������.
		, &sendBytes  // ����� ������������ ������.
		, 0  // �������������� �����.
		, (SOCKADDR*)&receiver  // ����� �������-���������.
		, sizeof (receiver)  // ����� ������ ���������.
		, NULL  // ��������� ��� ����������� �����-������.
		, NULL  // ��������� ��� ���������� ����������� �����-������.
	);
	
	if (SOCKET_ERROR == sendResult)
	{
		long errorCode = WSAGetLastError();
		printf("������ WSASendTo() � ����� = %d!\n", errorCode);
		printErrorMessage(errorCode);
		return errorCode;
	}
	
	printf("������� %d ������.\n", sendBytes);
	return sendResult;
}

void printErrorMessage(long errorCode)
{
	switch (errorCode)
	{
	case WSASYSNOTREADY:
		printf("���������� ���������� ��� ������ � ������� �����!\n");
		break;

	case WSAVERNOTSUPPORTED:
		printf("���������� ��������� �������� ������ ���������� �������!\n");
		break;

	case WSAEINPROGRESS:
		printf("������������� ����������� �������� ���������� ������� ������ 1.1!\n");
		break;

	case WSAEPROCLIM:
		printf("���������� ����������� �� ���������� ������� � �������� ������ ���������� �������!\n");
		break;

	case WSAEFAULT:
		printf("�������� �������� �������� lpWSAData!\n");
		break;
	
	case WSANOTINITIALISED:
		printf("��� �� ���������� ������� WSAStartup!\n");
		break;

	case WSAENETDOWN:
		printf("���� ����!\n");
		break;
	}
}