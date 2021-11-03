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

/**
 * Splits a given string into pieces around a delimiter
 *
 * @param toSplit string to split
 * @param delim string representing the delimiter to split on
 * @param count the number of segments the string was split into
 * @return a allocated char pointer to a char pointer array containing the split string parts
 */
char** str_split(const char* toSplit, char* delim, int* count);

#endif //UTIL_H
