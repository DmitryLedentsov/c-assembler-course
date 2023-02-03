/* task1.c */

#include <stdio.h>
#define print_var(x) printf(#x " is %d \n", x )

const int y = 6;
int main() {
    int x = 5;
    print_var(x);
    print_var(5);
    print_var(y);
    return 0;
}
