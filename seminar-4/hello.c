extern __attribute__((noinline)) void print_string(char* a);
extern __attribute__(()) void world();
extern __attribute((noinline)) void print_file();
#include <stdio.h>
/* hello.c */
int main() {
    printf("%s", "enter file name: ");
    char name [20];
    char *ptr = name;
    int i = scanf("%20s",ptr);
    ptr = name;
    if (i!=0) print_file(ptr);
}
