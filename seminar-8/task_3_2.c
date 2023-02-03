#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

void* create_shared_memory(size_t size) {
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                -1, 0);
}

pthread_t th1, th2;
sem_t sem_print, sem_end, sem_end_print;

int* shmem;
const int N = 10;

void* printf_th(void* args) {
    while (1) {
        int id, val;
        scanf("%d %d", &id, &val);
        if (id > N - 1 || id < 0) break;

        shmem[id] = val;
        sem_post(&sem_print);
        sem_wait(&sem_end_print);
    }

    sem_post(&sem_end);
    sem_post(&sem_print);

    return 0;
}

void* scanf_th(void* args) {
    while (1) {
        sem_wait(&sem_print);

        if (sem_trywait(&sem_end) == 0) break;

        for (int i = 0; i < N; i++) {
            printf("%d ", shmem[i]);
        }
        printf("\n");

        sem_post(&sem_end_print);
    }
    return 0;
}

int main() {
    shmem = create_shared_memory(sizeof(int) * N);

    for (size_t i = 0; i < N; i++) {
        shmem[i] = i;
    }

    sem_init(&sem_print, 1, 0);
    sem_init(&sem_end_print, 1, 0);
    sem_init(&sem_end, 1, 0);

    pthread_create(&th1, NULL, scanf_th, NULL);
    pthread_create(&th2, NULL, printf_th, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    sem_destroy(&sem_print);
    sem_destroy(&sem_end_print);
    sem_destroy(&sem_end);

    return 0;
}