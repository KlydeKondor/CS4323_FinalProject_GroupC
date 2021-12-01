#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "dataServerNetwork.h"
#include "serverNetwork.h"
#include "serverToDataserverAPI.h"
#include "socketConnection.h"
#include "util.h"
#include "HunterHolstead.h"

#define RETURN_DATA_INDEX 1

#define PACKED_DATA_SIZE 3
#define CLIENT_SOCKET_ID 0
#define CLIENT_SOCKET 1
#define SERVER_SOCKET 2

static int nextRoutingID = 0;
static struct socket_t* clientSockets[50];

_Noreturn void* clientToDataServerHandle(void* data) {
    void** unpackedData = (void**)data;
    int routingID = *(int*) unpackedData[CLIENT_SOCKET_ID];
    struct socket_t* clientSocket = (struct socket_t*) unpackedData[CLIENT_SOCKET];
    struct socket_t* dataServerSocket = (struct socket_t*) unpackedData[SERVER_SOCKET];

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE - 64];
        readSocket(clientSocket, buffer);

        char message[MAX_TCP_BUFFER_SIZE];
        sprintf(message, "%d%s%s", routingID, COMMAND_DELIMITER, buffer);
        writeSocket(dataServerSocket, message);
    }
}

_Noreturn void* DataServerToClientHandle(void* data) {
    struct socket_t* dataServerSocket = (struct socket_t *) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(dataServerSocket, buffer);

        int count;
        char** split = str_split(buffer, COMMAND_DELIMITER, &count);

        int routingID = atoi(split[ROUTING_ID_INDEX]);
        writeSocket(clientSockets[routingID], split[RETURN_DATA_INDEX]);
    }
}

void* threadSpawnHandle(void* data) {
    pthread_t clientToDataServerThread;

    pthread_create(&clientToDataServerThread, NULL, clientToDataServerHandle, data);
    pthread_join(clientToDataServerThread, NULL);

    void** unpackedData = (void**)data;
    struct socket_t* clientSocket = (struct socket_t*) unpackedData[CLIENT_SOCKET];
    struct socket_t* serverSocket = (struct socket_t*) unpackedData[SERVER_SOCKET];
    free(unpackedData[0]);
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

    pthread_t dataServerListen;
    pthread_create(&dataServerListen, NULL, DataServerToClientHandle, dataServerSocket);

    while(1) {
        // Accept incoming server connection and pass it off to a thread
        struct socket_t* acceptedSocket = acceptSocket(serverSocket);

        void** packedData = malloc(sizeof(void*) * PACKED_DATA_SIZE);
        int* value = malloc(sizeof(int));
        *value = nextRoutingID;
        packedData[CLIENT_SOCKET_ID] = value;
        clientSockets[nextRoutingID] = acceptedSocket;
        nextRoutingID++;
        packedData[CLIENT_SOCKET] = acceptedSocket;
        packedData[SERVER_SOCKET] = dataServerSocket;

        pthread_t clientServerThread;
        pthread_create(&clientServerThread, NULL, threadSpawnHandle, packedData);
    }
}