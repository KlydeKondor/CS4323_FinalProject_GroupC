#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void* clientToDataServerHandle(void* data) {
    void** unpackedData = (void**)data;
    int routingID = *(int*) unpackedData[CLIENT_SOCKET_ID];
    struct socket_t* clientSocket = (struct socket_t*) unpackedData[CLIENT_SOCKET];
    struct socket_t* dataServerSocket = (struct socket_t*) unpackedData[SERVER_SOCKET];

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE - 64];
        readSocket(clientSocket, buffer);

        printf("Read client traffic: %s\n", buffer);

        if(strcmp(buffer, QUIT) == 0) {
            break;
        }

        char message[MAX_TCP_BUFFER_SIZE];
        sprintf(message, "%d%s%s", routingID, COMMAND_DELIMITER, buffer);
        writeSocket(dataServerSocket, message);
    }

    free(unpackedData[CLIENT_SOCKET_ID]);
    freeSocket(unpackedData[CLIENT_SOCKET]);
    freeSocket(unpackedData[SERVER_SOCKET]);
    free(data);

    return NULL;
}

_Noreturn void* DataServerToClientHandle(void* data) {
    struct socket_t* dataServerSocket = (struct socket_t *) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(dataServerSocket, buffer);

        printf("Read data server traffic: %s\n", buffer);

        int count;
        char** split = str_split(buffer, COMMAND_DELIMITER, &count);

        int routingID = atoi(split[ROUTING_ID_INDEX]);
        writeSocket(clientSockets[routingID], split[RETURN_DATA_INDEX]);
    }
}
int main(int argc, char **argv) {
    seedRand();
    // Parse server port
    if(argc < 2) {
        printf("Port argument missing\nUsage: ./server.out <port>\n");
        exit(1);
    }

    int port = atoi(argv[1]);

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
        pthread_create(&clientServerThread, NULL, clientToDataServerHandle, packedData);
    }
}