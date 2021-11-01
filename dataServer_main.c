#include <pthread.h>
#include "dataServerNetwork.h"
#include "socketConnection.h"

_Noreturn void* serverListenHandle(void* data) {
    struct socket_t* clientServerSocket = (struct socket_t*) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(clientServerSocket, buffer);

    }
}

int main() {
    // Setup listening socket for server connection
    struct socket_t* dataServerSocket = mallocSocket(DATA_SERVER_ADDRESS, DATA_SERVER_PORT);
    bindSocket(dataServerSocket);
    listenSocket(dataServerSocket, 3);

    while(1) {
        // Accept incoming server connection and pass it off to a thread
        struct socket_t* acceptedSocket = acceptSocket(dataServerSocket);
        pthread_t clientServerThread;
        pthread_create(&clientServerThread, NULL, serverListenHandle, acceptedSocket);
    }
}