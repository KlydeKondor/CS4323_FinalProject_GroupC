#include <stdio.h>
#include <stdlib.h>
#import "serverNetwork.h"
#include "socketConnection.h"

int main(int argc, char **argv) {
    // Parse server port
    if(argc < 1) {
        printf("Port argument missing\nUsage: ./client.out <port>\n");
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

    // Connect to the server
    struct socket_t* dataServerSocket = mallocSocket(SERVER_ADDRESS, port);
    connectSocket(dataServerSocket, 5, 2);
}