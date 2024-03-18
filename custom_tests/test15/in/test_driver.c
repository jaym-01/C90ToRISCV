#include <stdio.h>

int test(int *x);

int f(){
    return 200;
}

int main() {
    int x = 100;
    int *y = &x;
    printf("Test 1: %d\n", test(y));

    return 0;
}
