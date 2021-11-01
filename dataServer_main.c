#include <signal.h>
#include <stdio.h>
#include "dataServer.h"
#include "socketConnection.h"

_Noreturn void* serverListenHandle(void* data) {

}

int main() {
    struct socket_t* dataServerSocket = mallocSocket(DATA_SERVER_ADDRESS, DATA_SERVER_PORT);
    bindSocket(dataServerSocket);
    listenSocket(dataServerSocket, 3);

    while(1) {
        struct socket_t* acceptedSocket = acceptSocket(dataServerSocket);

    }
}