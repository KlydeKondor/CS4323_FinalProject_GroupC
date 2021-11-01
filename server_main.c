#include <pthread.h>
#include "dataServerNetwork.h"
#include "serverNetwork.h"
#include "socketConnection.h"

_Noreturn void* serverListenHandle(void* data) {
    struct socket_t* clientSocket = (struct socket_t*) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(clientSocket, buffer);

        // Handling of client traffic will go here
    }
}

int main() {
    // Connect to the data server
    struct socket_t* dataServerSocket = mallocSocket(DATA_SERVER_ADDRESS, DATA_SERVER_PORT);
    connectSocket(dataServerSocket, 5, 2);

    // Setup listen socket to handle incoming client connections
    struct socket_t* serverSocket = mallocSocket(SERVER_ADDRESS, SERVER_PORT);
    bindSocket(serverSocket);
    listenSocket(serverSocket, 3);

    while(1) {
        // Accept incoming server connection and pass it off to a thread
        struct socket_t* acceptedSocket = acceptSocket(serverSocket);
        pthread_t clientServerThread;
        pthread_create(&clientServerThread, NULL, serverListenHandle, acceptedSocket);
    }
}