#include <pthread.h>
#include "dataServerNetwork.h"
#include "serverToDataserverAPI.h"
#include "socketConnection.h"
#include "util.h"

// Kyle: Test cases for Kyle
#include "database.h"

static void writeOther(struct socket_t* socket, char* routingID, char* other) {
    assert(socket != NULL);
    assert(routingID != NULL);
    assert(other != NULL);

    char buffer[MAX_TCP_BUFFER_SIZE];
    sprintf(buffer, "%s%s%s", routingID, COMMAND_DELIMITER, other);
    writeSocket(socket, buffer);
}

static void writeSuccess(struct socket_t* socket, char* routingID) {
    assert(socket != NULL);
    assert(routingID != NULL);

    writeOther(socket, routingID, COMMAND_SUCCESS);
}

static void writeFailure(struct socket_t* socket, char* routingID) {
    assert(socket != NULL);
    assert(routingID != NULL);

    writeOther(socket, routingID, COMMAND_FAILURE);
}

_Noreturn void* serverListenHandle(void* data) {
    struct socket_t* clientServerSocket = (struct socket_t*) data;

    while(1) {
        char buffer[MAX_TCP_BUFFER_SIZE];
        readSocket(clientServerSocket, buffer);

        int count;
        char** split = str_split(buffer, COMMAND_DELIMITER, &count);

        char* routingID = split[ROUTING_ID_INDEX];
        char* command = split[COMMAND_INDEX];
        char* packetData = split[DATA_INDEX];

        if(strcmp(command, REGISTER_CUSTOMER) == 0) {
            int success = registerClient(packetData, 1);
            if(success == 0) {
                writeSuccess(clientServerSocket, routingID);
            }
            else {
                writeFailure(clientServerSocket, routingID);
            }
        }
        else if(strcmp(command, UPDATE_CUSTOMER) == 0) {
            int success = updateClient(packetData, 1);
            if(success == 0) {
                writeSuccess(clientServerSocket, routingID);
            }
            else {
                writeFailure(clientServerSocket, routingID);
            }
        }
        else if(strcmp(command, REGISTER_SELLER) == 0) {
            int success = registerClient(packetData, 0);
            if(success == 0) {
                writeSuccess(clientServerSocket, routingID);
            }
            else {
                writeFailure(clientServerSocket, routingID);
            }
        }
        else if(strcmp(command, UPDATE_SELLER) == 0) {
            int success = updateClient(packetData, 0);
            if(success == 0) {
                writeSuccess(clientServerSocket, routingID);
            }
            else {
                writeFailure(clientServerSocket, routingID);
            }
        }
        else if(strcmp(command, ADD_PRODUCT) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, UPDATE_PRODUCT) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, DELETE_PRODUCT) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, UPDATE_PRODUCT_QUANTITY) == 0) {
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, UPDATE_PRODUCT_PRICE) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, UPDATE_BILLING_INFO) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, ADD_ORDER) == 0) {
            // TODO
            writeFailure(clientServerSocket, routingID);
        }
        else if(strcmp(command, GET_SELLER_PRODUCTS) == 0) {
            char* info = viewProductsSeller(packetData);
            writeSocket(clientServerSocket, info);
            free(info);
        }
        else if(strcmp(command, GET_PRODUCT_INFO) == 0) {
            char* info = viewProductsBuyer(packetData);
            writeOther(clientServerSocket, routingID, info);
            free(info);
        }
        else if(strcmp(command, GET_SELLER_ORDERS) == 0) {
            char* info = viewOrdersSeller(packetData);
            writeOther(clientServerSocket, routingID, info);
            free(info);
        }
        else if(strcmp(command, GET_BUY_ORDERS) == 0) {
            char* info = viewOrdersBuyer(packetData);
            writeOther(clientServerSocket, routingID, info);
            free(info);
        }
        else if(strcmp(command, GET_BILLING_INFO) == 0) {
            char* info = viewBillingInfo(packetData);
            writeOther(clientServerSocket, routingID, info);
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