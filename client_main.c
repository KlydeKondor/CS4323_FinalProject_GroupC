#include <stdio.h>
#include <stdlib.h>
#include "HunterHolstead.h"
#include "serverNetwork.h"
#include "socketConnection.h"
#include "util.h"

int main(int argc, char **argv) {
    seedRand();
    // Parse server port
    if(argc < 2) {
        printf("Port argument missing\nUsage: ./client.out <port>\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    // Connect to the server
    struct socket_t* serverSocket = mallocSocket(SERVER_ADDRESS, port);
    connectSocket(serverSocket, 5, 2);

    RunClient(serverSocket);
}