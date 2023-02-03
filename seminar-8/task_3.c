/* fork-example-1.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stddef.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>

void *create_shared_memory(size_t size) {
    return mmap(NULL,
                size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS,
                -1, 0);
}

void print_int_array(int *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (i > 0) {
            printf(", ");
        } else if (i == 0) {
            printf("[");
        }
        printf("\"%d\"", array[i]);
        if (i == 9) {
            printf("]");
        }
    }
    printf("\n");
}
int parent_proc();
int child_proc();

/*
 * Написать синхронизацию с помощью pipes, при этом программа будет работать, пока не будет
 * введено отрицательное значение индекса.
 * 
 * */

sem_t sem_print, sem_end_print, sem_end;
int *integers = NULL;
int pid;
int my_index = 0;
int is_work = 1;
int N=9;
void* shmem;
int parent_proc(void) {
    for (;;) {
        sem_wait(&sem_print);
        if(sem_trywait(&sem_end)==0) break;
        printf("shared data");
        for(int i=0; i<N; i++) printf("%d ", *((int *) shmem+i));
        printf("\n");
    }
    sem_post(&sem_end_print);
    return 0;
}

int child_proc(void){
    for(;;){
        printf("input pair [id,value]");
        fflush(stdout);
        int id, val;
        scanf("%d %d", &id, &val);
        //printf("%d %d", id, val);
        if(id>N-1 || id<0) break;
        *((int *) shmem + id) = val;
        sem_post(&sem_print);
        sem_wait(&sem_end_print);
    }
    sem_post(&sem_end);
    sem_post(&sem_print);
    return 0;
}

int main() {
    sem_init(&sem_print,1,0);
    sem_init(&sem_end_print, 1, 0);
    sem_init(&sem_end,1,0);
    shmem = create_shared_memory(sizeof(int) * 10);
    if (shmem == MAP_FAILED) {
        printf("Ошибка маппинга. Ты придурок.");
    }
    integers = shmem;
    integers[0] = 0;
    integers[1] = 1;
    integers[2] = 2;
    integers[3] = 3;
    integers[4] = 4;
    integers[5] = 5;
    integers[6] = 6;
    integers[7] = 7;
    integers[8] = 8;
    integers[9] = 9;

    int pid = fork();
    if(pid==0){
        return child_proc();
    }
    int ret = parent_proc();
    waitpid(pid,NULL, 0);
    return ret;
}
