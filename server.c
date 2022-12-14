#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "score.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT 27015
#define DEFAULT_BUFLEN 512

int __cdecl main() {
    int iResult;

    WSADATA wsaData;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DEFAULT_PORT);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;

    iResult = bind(ListenSocket, &sin, sizeof(sin));
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Start listening
    if (listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    struct sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);

    SOCKET ClientSocket = INVALID_SOCKET;

    ClientSocket = accept(ListenSocket, &remoteAddr, &nAddrLen);

    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accepted a connection
    printf("Accepted a connection: %s\n", inet_ntoa(remoteAddr.sin_addr));

    // No longer need server socket
    closesocket(ListenSocket);

    char recvbuf[DEFAULT_BUFLEN];
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    const char* szText = "TCP server connected!\n";
    send(ClientSocket, szText, strlen(szText), 0);

    score t[10];
    int cnt = 0;

    // Receive until the peer shuts down the connection
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            char* s = recvbuf;

            // The server may receive multiple records, each ended with a '\n'
            do {
                parse_score_record_str(s, &t[cnt]);
                printf("ID: %d; Name: %s; Score: %d\n", t[cnt].id, t[cnt].name, t[cnt].score);
                cnt += 1;
                while (*s != '\n') ++s;
                ++s;
            } while (s - recvbuf < iResult);
            
            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Echoing received content...\n");
            printf("Bytes sent: %d\n", iSendResult);

            if (cnt == sizeof(t) / sizeof(t[0])) break;
        } else if (iResult == 0) {
            printf("Connection closing...\n");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    // Write received data to CSV file
    FILE* out = fopen("data.csv", "w");
    
    fprintf(out, "Id,Name,Score\n");
    for (int i = 0; i < cnt; ++i) {
        fprintf(out, "%d,%s,%d\n", t[i].id, t[i].name, t[i].score);
    }

    fclose(out);

    return 0;
}
