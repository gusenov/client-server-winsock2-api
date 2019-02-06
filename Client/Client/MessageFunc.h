// Код, реализующий операции по обмену сообщениями:

#ifndef MESSAGEFUNC_H
#define MESSAGEFUNC_H

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define RECEIVE_BUFFER_DEFAULT_MAX_LEN 256

long loadWinSockLibrary();
long unloadWinSockLibrary();

long createDatagramSocket(SOCKET* socket);
void closeSocket(SOCKET socket);

long bindSocket(SOCKET unboundSocket, INT port);

long receiveMessage(SOCKET socket, char* buffer, ULONG bufferSize, char** senderIp);
long sendMessage(SOCKET socket, char* message, const char* ip, INT port);

void printErrorMessage(long errorStatus);

#endif
