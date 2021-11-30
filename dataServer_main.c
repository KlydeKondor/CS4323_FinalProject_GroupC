#include <pthread.h>
#include "dataServerNetwork.h"
#include "serverToDataserverAPI.h"
#include "socketConnection.h"
#include "util.h"

// Kyle: Test cases for Kyle
#include "database.h"

_Noreturn void* serverListenHandle(void* data) {
    struct socket_t* clientServerSocket = (struct socket_t*) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(clientServerSocket, buffer);

        int count;
        char** split = str_split(buffer, COMMAND_DELIMITER, &count);

        if(count < 2) {
            writeSocket(clientServerSocket, COMMAND_FAIL);
            continue;
        }

        char* command = split[0];

        if(strcmp(command, REGISTER_CLIENT) == 0) {

        }
        else if(strcmp(command, UPDATE_CLIENT) == 0) {

        }
        else if(strcmp(command, ADD_PRODUCT) == 0) {

        }
        else if(strcmp(command, UPDATE_PRODUCT) == 0) {

        }
        else if(strcmp(command, DELETE_PRODUCT) == 0) {

        }
        else if(strcmp(command, UPDATE_PRODUCT_QUANTITY) == 0) {

        }
        else if(strcmp(command, UPDATE_PRODUCT_PRICE) == 0) {

        }
        else if(strcmp(command, UPDATE_BILLING_INFO) == 0) {

        }
        else if(strcmp(command, ADD_ORDER) == 0) {

        }
        else if(strcmp(command, GET_SELLER_PRODUCTS) == 0) {

        }
        else if(strcmp(command, GET_PRODUCT_INFO) == 0) {

        }
        else if(strcmp(command, GET_SELLER_ORDERS) == 0) {

        }
        else if(strcmp(command, GET_BUY_ORDERS) == 0) {

        }
        else if(strcmp(command, GET_BILLING_INFO) == 0) {

        }
    }
}

int main() {
    seedRand();
    // Setup listening socket for server connection
    struct socket_t* dataServerSocket = mallocSocket(DATA_SERVER_ADDRESS, DATA_SERVER_PORT);
    bindSocket(dataServerSocket);
    listenSocket(dataServerSocket, 3);
	
	// Kyle's test cases (location is temporary)
	char dummyData[] = "5|Jane Doe|555-867-5309|123 Address Road|";
	registerClient(dummyData, 1); // INSERT
	
	char dummyUpdate[] = "5|tesT|Dadt|Foo|";
	updateClient(dummyUpdate, 1); // UPDATE
	
    while(1) {
        // Accept incoming server connection and pass it off to a thread
        struct socket_t* acceptedSocket = acceptSocket(dataServerSocket);
        pthread_t clientServerThread;
        pthread_create(&clientServerThread, NULL, serverListenHandle, acceptedSocket);
    }
}