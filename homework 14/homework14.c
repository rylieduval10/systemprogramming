#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int total_number = 100; // p
int thread_total = 4;   // n

void *task(void *arg) {
    long index = (long)arg;
    int chunk_size = total_number / thread_total;
    if (total_number % thread_total != 0) {
        chunk_size = chunk_size + 1;
    }

    int start = chunk_size * index + 1;
    int end = chunk_size * (index + 1);
    if (index + 1 == thread_total) {
        end = total_number;
    }

    for (int i = start; i <= end; i++) {
        printf("thread id %ld, count %d\n", index, i);
    }

    return NULL;
}

int main() {
    pthread_t threads[thread_total];

    for (long i = 0; i < thread_total; i++) {
        pthread_create(&threads[i], NULL, task, (void *)i);
    }

    for (int i = 0; i < thread_total; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
