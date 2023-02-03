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
        printf("%d", array[i]);
    }
    printf("\n");
}

int main() {
    int *integers = NULL;
    void *shmem = create_shared_memory(sizeof(int) * 10);
    if (shmem == MAP_FAILED) {
        printf("mapping error");
    }
    integers = shmem;
    *integers = (int*){0,1,2,3,4,5,6,7,8,9};


    printf("Shared memory at: %p\n", integers);
    int pid = fork();

    int index = 0;
    int new_num = 0;

    if (pid == 0) { // if child
        printf("Child's pid is: %d\n", pid);
        scanf("%d", &index);
        scanf("%d", &new_num);
        integers[index] = new_num;
    } else {
        wait(NULL);
        print_int_array(integers, 10);
    }
    return 0;
}