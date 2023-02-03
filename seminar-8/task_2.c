/* fork-example-1.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stddef.h>

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
        printf("%d", array[i]);
        if (i == size-1) {
            printf("]");
        }
    }
    printf("\n");
}

/*
 * Написать синхронизацию с помощью pipes, при этом программа будет работать, пока не будет
 * введено отрицательное значение индекса.
 * */


int main() {

    int pipes[2][2];
    pipe(pipes[0]);
    pipe(pipes[1]);

    int *integers = NULL;
    void *shmem = create_shared_memory(sizeof(int) * 10);
    if (shmem == MAP_FAILED) {
        printf("mapping error.");
    }
    integers = shmem;
    /*integers[0] = 0;
    integers[1] = 1;
    integers[2] = 2;
    integers[3] = 3;
    integers[4] = 4;
    integers[5] = 5;
    integers[6] = 6;
    integers[7] = 7;
    integers[8] = 8;
    integers[9] = 9;*/
    for(int i =0; i<=9;i++) integers[i]=i;


    printf("Shared memory at: %p\n", integers);


    int pid = fork();

    if (pid == 0) {
        int to_parent_pipe = pipes[1][1];
        int from_parent_pipe = pipes[0][0];
        close(pipes[1][0]);
        close(pipes[0][1]);
        int index = 0;
        int new_num = 0;
        while(1) {
            scanf("%d", &index);
            scanf("%d", &new_num);
            if (index < 0) {
                write(to_parent_pipe, &index, sizeof(int));
                break;
            }
            integers[index] = new_num;
            write(to_parent_pipe, &index, sizeof(int));
        }

        close(to_parent_pipe);
        close(from_parent_pipe);
        return 0;
    }

    int from_child_pipe = pipes[1][0];
    int to_child_pipe = pipes[0][1];

    close(pipes[1][1]);
    close(pipes[0][0]);
    int index;

    printf("Родительский процесс\n", pid);
    while (1) {
        int ret;
        int addr=1;
        while (ret = read(from_child_pipe, &addr, 4) == 0);
        if (index < 0) {
            printf("Отправка сообщений закончена.");
            break;
        }
        printf("Данные изменены!!!\n");
        print_int_array(integers, 10);
    }

    waitpid(pid, NULL, 0);

    // Закроем дескпиторы
    close(from_child_pipe);
    close(to_child_pipe);

    return 0;
}
