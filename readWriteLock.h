#include <semaphore.h>

#ifndef CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H
#define CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H

/**
 * Enum representing semaphore lock mode
 */
enum LockMode { THREADS = 0, PROCESSES = 1};

/**
 * A lock containing independent read and write locks. Any direct modification to struct fields is undefined behaviour.
 */
struct RWLock_t {
    int readCount;
    sem_t readCountLock;
    sem_t writeLock;
};

/**
 * Allocates a new read/write lock
 * @param mode The mode the lock will operate in
 * @return A pointer to the allocated lock
 */
struct RWLock_t* mallocRWLock(enum LockMode mode);

/**
 * Locks the read lock
 * @param lock struct to lock
 */
void readLock(struct RWLock_t* lock);

/**
 * Unlocks the read lock
 * @param lock struct to unlock
 */
void readUnlock(struct RWLock_t* lock);

/**
 * Locks the write lock
 * @param lock struct to lock
 */
void writeLock(struct RWLock_t* lock);

/**
 * Unlocks the write lock
 * @param lock struct to unlock
 */
void writeUnlock(struct RWLock_t* lock);

/**
 * Frees an allocated read/write lock
 * @param lock lock to free
 */
void freeRWLock(struct RWLock_t* lock);

#endif //CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H
