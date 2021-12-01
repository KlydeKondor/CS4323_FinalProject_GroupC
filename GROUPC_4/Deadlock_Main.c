#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lockA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockB = PTHREAD_MUTEX_INITIALIZER;

void* threadA(void* data) {
    while (1) {
        pthread_mutex_lock(&lockA);
        sleep(1); // Gives time for the other thread to acquire the other lock
        pthread_mutex_lock(&lockB);
        printf("THREAD A WORK DONE\n");
        pthread_mutex_unlock(&lockB);
        pthread_mutex_unlock(&lockA);
        sleep(1); // Allows for some randomization in which thread acquired the next set of locks
    }
}

void* threadB(void* data) {
    while (1) {
        pthread_mutex_lock(&lockA);
        sleep(1); // Gives time for the other thread to acquire the other lock
        pthread_mutex_lock(&lockB);
        printf("THREAD B WORK DONE\n");
        pthread_mutex_unlock(&lockB);
        pthread_mutex_unlock(&lockA);
        sleep(1); // Allows for some randomization in which thread acquired the next set of locks
    }
}

int main() {
    pthread_t A, B;

    pthread_create(&A, NULL, threadA, NULL);
    pthread_create(&B, NULL, threadB, NULL);

    pthread_join(A, NULL);
    pthread_join(B, NULL);
}