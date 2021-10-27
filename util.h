#ifndef UTIL_H
#define UTIL_H

/**
 * Allocates an empty string with the specified length, including a null-terminator at length + 1
 *
 * @param length the desired length of the string
 * @return A pointer to the first element of the string
 */
char* mallocString(unsigned int length);

/**
 * Formats an address and port to a sockaddr_in with default parameters
 *
 * @param address the desired address
 * @param port the desired port
 * @return A sockaddr_in struct containing the address and port
 */
struct sockaddr_in addressAndPort(const char* address, int port);

/**
 * Seeds rand() with the current system time
 */
void seedRand();

#endif //UTIL_H
