#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#define MAX_TCP_BUFFER_SIZE 8192

/**
 * A struct representing a wrapper around a C-style int socket
 */
struct socket_t {
    const int socketFD;
    const char address[20];
    const int port;
};

/**
 * Mallocs a new socket.
 *
 * @param address the address of the socket
 * @param port the port of the socket
 * @return A pointer to the created socket
 */
struct socket_t* mallocSocket(const char* address, int port);

/**
 * Binds a socket to the address and port
 * @param socket
 */
void bindSocket(const struct socket_t* socket);

/**
 * Sets a socket to listen on its specified address and port
 *
 * @param socket the socket to set up to listen
 * @param listenCount the maximum number of pending connections the socket can hold in queue
 */
void listenSocket(const struct socket_t* socket, int listenCount);

/**
 * Have a socket accept an incoming connection. This will block until a connection is established
 *
 * @param socket the server socket that will accept an incoming connection
 * @return A pointer to the socket that holds the incoming connection
 */
struct socket_t* acceptSocket(const struct socket_t* socket);

/**
 * Attempts to connect socket
 *
 * @param socket the socket that will try to connect
 * @param maxRetryAttempt the maximum amount  of times the socket will try to connect before failing
 * @param backoffFactor scales how long the socket should wait after each failed attempt. A value of 1 will have the socket wait one additional second after each failed attempt.
 */
void connectSocket(const struct socket_t* socket, int maxRetryAttempt, float backoffFactor);

/**
 * Has a socket read data. This will block until data is received.
 *
 * @param socket the socket to read from
 * @param buffer a buffer to put the received data
 */
void readSocket(const struct socket_t* socket, char* buffer);

/**
 * Has a socket write data.
 *
 * @param socket the socket to write to
 * @param buffer the data that will be writen to the socket
 */
void writeSocket(const struct socket_t* socket, char* buffer);

/**
 * Closes and frees a socket connection.
 *
 * @param socket the socket to free
 */
void freeSocket(struct socket_t* socket);

#endif //SOCKETCONNECTION_H
