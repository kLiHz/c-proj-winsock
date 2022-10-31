#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>

#include "score.h"

#define DEFAULT_PORT 27015
#define DEFAULT_BUFLEN 512

int main() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Connect to server.
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    iResult = connect(ConnectSocket, &serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN];
    const char *sendbuf = "this is a test";

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);

    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            recvbuf[iResult] = 0;
            printf("-- %s\n", recvbuf);
        } else if (iResult == 0) {
            printf("Connection closed\n");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
        }
    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
