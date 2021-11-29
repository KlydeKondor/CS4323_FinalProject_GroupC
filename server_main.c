#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "dataServerNetwork.h"
#include "serverNetwork.h"
#include "serverToDataserverAPI.h"
#include "socketConnection.h"
#include "util.h"

// Kyle: Test cases for Hunter
#include "HunterHolstead.h"

#define PACKED_DATA_SIZE 2
#define CLIENT_SOCKET 0
#define SERVER_SOCKET 1

void* clientToDataServerHandle(void* data) {
    void** unpackedData = (void**)data;
    struct socket_t* clientSocket = (struct socket_t*) unpackedData[CLIENT_SOCKET];
    struct socket_t* dataServerSocket = (struct socket_t*) unpackedData[SERVER_SOCKET];

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(clientSocket, buffer);

        // Handling of client traffic to the server will go here
        // RegisterClient(); This appears to be for client input, this should go on the client_main not the server
    }

    return NULL;
}

void* threadSpawnHandle(void* data) {
    pthread_t clientToDataServerThread;

    pthread_create(&clientToDataServerThread, NULL, clientToDataServerHandle, data);
    pthread_join(clientToDataServerThread, NULL);

    void** unpackedData = (void**)data;
    struct socket_t* clientSocket = (struct socket_t*) unpackedData[CLIENT_SOCKET];
    struct socket_t* serverSocket = (struct socket_t*) unpackedData[SERVER_SOCKET];
    freeSocket(clientSocket);
    freeSocket(serverSocket);
    free(data);

    return NULL;
}

int main(int argc, char **argv) {
    seedRand();
    // Parse server port
    if(argc < 1) {
        printf("Port argument missing\nUsage: ./server.out <port>\n");
        exit(1);
    }
    long port;
    char* other;
    if((port = strtol(argv[0], &other, 10)) == 0) {
        if(port > (1 << 16)) { //Max port: 65536
            printf("Port outside of valid range 0 < port < 65536\n");
            exit(1);
        }
    }

    // Connect to the data server
    struct socket_t* dataServerSocket = mallocSocket(DATA_SERVER_ADDRESS, DATA_SERVER_PORT);
    connectSocket(dataServerSocket, 5, 2);

    // Setup listen socket to handle incoming client connections
    struct socket_t* serverSocket = mallocSocket(SERVER_ADDRESS, port);
    bindSocket(serverSocket);
    listenSocket(serverSocket, 3);

    while(1) {
        // Accept incoming server connection and pass it off to a thread
        struct socket_t* acceptedSocket = acceptSocket(serverSocket);

        void** packedData = malloc(sizeof(void*) * PACKED_DATA_SIZE);
        packedData[CLIENT_SOCKET] = acceptedSocket;
        packedData[SERVER_SOCKET] = dataServerSocket;

        pthread_t clientServerThread;
        pthread_create(&clientServerThread, NULL, threadSpawnHandle, packedData);
    }
}