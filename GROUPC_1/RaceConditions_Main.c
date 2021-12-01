#include <pthread.h>
#include <stdio.h>

#define LOOP_COUNT 10000

int counter;

void* threadHandle(void* data) {
    for(int i = 0; i < LOOP_COUNT; i++) {
        counter++;
    }

    return NULL;
}

int main() {
    printf("Ideal output: %d\n", 2 * LOOP_COUNT);

    for(int i = 0; i < 10; i++) {
        counter = 0;

        pthread_t handle1;
        pthread_t handle2;

        pthread_create(&handle1, NULL, threadHandle, NULL);
        pthread_create(&handle2, NULL, threadHandle, NULL);

        pthread_join(handle1, NULL);
        pthread_join(handle2, NULL);

        printf("Execution %d: %d\n", i + 1, counter);
    }
}