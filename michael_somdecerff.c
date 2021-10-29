#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "readWriteLock.h"
#include "socketConnection.h"
#include "util.h"

///////////////////////////////////////////////
//
// ReadWriteLock.h
//
///////////////////////////////////////////////
struct RWLock_t* mallocRWLock(enum LockMode mode) {
    struct RWLock_t* lock = malloc(sizeof(struct RWLock_t));
    lock->readCount = 0;

    if(sem_init(&lock->readCountLock, mode, 1) == -1) {
        printf("[mallocRWLock] Init of readCountLock failed");
        exit(1);
    }

    if(sem_init(&lock->writeLock, mode, 1) == -1) {
        printf("[mallocRWLock] Init of writeLock failed");
        exit(1);
    }

    return lock;
}

void readLock(struct RWLock_t* lock) {
    assert(lock != NULL);

    sem_wait(&lock->readCountLock);
    lock->readCount++;
    if(lock->readCount == 1) {
        sem_wait(&lock->writeLock);
    }
    sem_post(&lock->readCountLock);
}

void readUnlock(struct RWLock_t* lock) {
    assert(lock != NULL);

    sem_wait(&lock->readCountLock);
    lock->readCount--;
    if(lock->readCount == 0) {
        sem_post(&lock->writeLock);
    }
    sem_post(&lock->readCountLock);
}

void writeLock(struct RWLock_t* lock) {
    assert(lock != NULL);
    sem_wait(&lock->writeLock);
}

void writeUnlock(struct RWLock_t* lock) {
    assert(lock != NULL);
    sem_post(&lock->writeLock);
}

void freeRWLock(struct RWLock_t* lock) {
    assert(lock != NULL);

    sem_destroy(&lock->readCountLock);
    sem_destroy(&lock->writeLock);
    free(lock);
    lock = NULL;
}

///////////////////////////////////////////////
//
// SocketConnection.h
//
///////////////////////////////////////////////
#define MILLISECONDS_PER_SECOND 1000

struct socket_t* mallocSocket(const char* address, int port) {
    struct socket_t* created = malloc(sizeof(struct socket_t));

    // Memory cast hack to remove the const on the struct field to assign value
    *(int *)&created->socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(created->socketFD == -1) {
        printf("[mallocSocket] creation of socket failed\n");
        exit(1);
    }

    strcpy((char*)created->address, address);
    *(int *)&created->port = port;

    return created;
}

void bindSocket(const struct socket_t* socket) {
    assert(socket != NULL);

    struct sockaddr_in serverAddress = addressAndPort(socket->address, socket->port);
    if(bind(socket->socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
        printf("[bindSocket] failed to bind to port\n");
        exit(1);
    }
}

void listenSocket(const struct socket_t* socket, int listenCount) {
    assert(socket != NULL);

    if(listen(socket->socketFD, listenCount) != 0) {
        printf("[listenAcceptSocket] failed to listen on socket\n");
        exit(1);
    }
}

struct socket_t* acceptSocket(const struct socket_t* socket) {
    assert(socket != NULL);

    struct sockaddr_in serverAddress = addressAndPort(socket->address, socket->port);

    int acceptedConnection;
    unsigned int length = sizeof(serverAddress);
    if((acceptedConnection = accept(socket->socketFD, (struct sockaddr*)&serverAddress, &length)) < 0) {
        printf("[listenAcceptSocket] failed to accept incoming client connection\n");
        exit(1);
    }

    struct socket_t* clientSocket = malloc(sizeof(struct socket_t));
    *(int*)&clientSocket->socketFD = acceptedConnection;

    return clientSocket;
}

void connectSocket(const struct socket_t* socket, int maxRetryAttempt, float backoffFactor) {
    assert(socket != NULL);
    assert(maxRetryAttempt >= 0);
    assert(backoffFactor >= 0);

    struct sockaddr_in serverAddress = addressAndPort(socket->address, socket->port);

    for(int i = 0; i < maxRetryAttempt + 1; i++) {
        printf("Connecting to TCP server. Attempt %d...\n", i + 1);
        if (connect(socket->socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
            if(i != maxRetryAttempt) {
                float backoff = backoffFactor * (float)(i + 1);
                printf("Failed to connect, backoff waiting %.2f seconds\n", backoff);
                // For some reason usleep doesn't work here?
                sleep(MILLISECONDS_PER_SECOND * backoff);
                continue;
            }
            else {
                printf("[connectSocket] Failed to connect to server after %d retry attempts. Exiting...", maxRetryAttempt);
                exit(1);
            }
        }
        else {
            printf("connected to server successfully\n");
            break;
        }
    }
}

void readSocket(const struct socket_t* socket, char* buffer) {
    assert(socket != NULL);
    assert(buffer != NULL);

    if(read(socket->socketFD, buffer, MAX_TCP_BUFFER_SIZE) == -1) {
        printf("[readSocket] read failed\n");
        exit(1);
    }
}

void writeSocket(const struct socket_t* socket, char* buffer) {
    assert(socket != NULL);
    assert(buffer != NULL);

    if(write(socket->socketFD, buffer, MAX_TCP_BUFFER_SIZE) == -1) {
        printf("[writeSocket] write failed\n");
        exit(1);
    }
}

void freeSocket(struct socket_t* socket) {
    assert(socket != NULL);

    close(socket->socketFD);
    free(socket);
    socket = NULL;
}


///////////////////////////////////////////////
//
// Util.h
//
///////////////////////////////////////////////
char* mallocString(unsigned int length) {
    assert(length > 0);

    char* str = calloc(length + 1, sizeof(char));
    str[length] = '\0';

    return str;
}

struct sockaddr_in addressAndPort(const char* address, int port) {
    assert(address != NULL);
    assert(port > 0);

    struct sockaddr_in socketAddress;
    bzero(&socketAddress, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr(address);
    socketAddress.sin_port = htons(port);

    return socketAddress;
}

void seedRand() {
    srand(time(NULL));
}