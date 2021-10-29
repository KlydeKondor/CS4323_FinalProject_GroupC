#include <semaphore.h>

#ifndef CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H
#define CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H

enum LockMode { THREADS = 0, PROCESSES = 1};

struct RWLock_t {
    int readCount;
    sem_t readCountLock;
    sem_t writeLock;
};

struct RWLock_t* mallocRWLock(enum LockMode mode);
void readLock(struct RWLock_t* lock);
void readUnlock(struct RWLock_t* lock);
void writeLock(struct RWLock_t* lock);
void writeUnlock(struct RWLock_t* lock);
void freeRWLock(struct RWLock_t* lock);

#endif //CS4323_FINALPROJECT_GROUPC_READWRITELOCK_H
