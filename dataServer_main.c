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
            writeSocket(clientServerSocket, COMMAND_FAILURE);
            continue;
        }

        char* command = split[0];

        if(strcmp(command, REGISTER_CUSTOMER) == 0) {
            int success = registerClient(split[1], 1);
            if(success == 0) {
                writeSocket(clientServerSocket, COMMAND_SUCCESS);
            }
            else {
                writeSocket(clientServerSocket, COMMAND_FAILURE);
            }
        }
        else if(strcmp(command, UPDATE_CUSTOMER) == 0) {
            int success = updateClient(split[1], 1);
            if(success == 0) {
                writeSocket(clientServerSocket, COMMAND_SUCCESS);
            }
            else {
                writeSocket(clientServerSocket, COMMAND_FAILURE);
            }
        }
        else if(strcmp(command, REGISTER_SELLER) == 0) {
            int success = registerClient(split[1], 0);
            if(success == 0) {
                writeSocket(clientServerSocket, COMMAND_SUCCESS);
            }
            else {
                writeSocket(clientServerSocket, COMMAND_FAILURE);
            }
        }
        else if(strcmp(command, UPDATE_SELLER) == 0) {
            int success = updateClient(split[1], 0);
            if(success == 0) {
                writeSocket(clientServerSocket, COMMAND_SUCCESS);
            }
            else {
                writeSocket(clientServerSocket, COMMAND_FAILURE);
            }
        }
        else if(strcmp(command, ADD_PRODUCT) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, UPDATE_PRODUCT) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, DELETE_PRODUCT) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, UPDATE_PRODUCT_QUANTITY) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, UPDATE_PRODUCT_PRICE) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, UPDATE_BILLING_INFO) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, ADD_ORDER) == 0) {
            writeSocket(clientServerSocket, COMMAND_FAILURE);
        }
        else if(strcmp(command, GET_SELLER_PRODUCTS) == 0) {
            char* info = viewProductsSeller(split[1]);
            writeSocket(clientServerSocket, info);
            free(info);
        }
        else if(strcmp(command, GET_PRODUCT_INFO) == 0) {
            char* info = viewProductsBuyer(split[1]);
            writeSocket(clientServerSocket, info);
            free(info);
        }
        else if(strcmp(command, GET_SELLER_ORDERS) == 0) {
            char* info = viewOrdersSeller(split[1]);
            writeSocket(clientServerSocket, info);
            free(info);
        }
        else if(strcmp(command, GET_BUY_ORDERS) == 0) {
            char* info = viewOrdersBuyer(split[1]);
            writeSocket(clientServerSocket, info);
            free(info);
        }
        else if(strcmp(command, GET_BILLING_INFO) == 0) {
            char* info = viewBillingInfo(split[1]);
            writeSocket(clientServerSocket, info);
            free(info);
        }

        for(int i = 0; i < count; i++) {
            free(split[i]);
        }
        free(split);
    }
}

int main() {
    seedRand();
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