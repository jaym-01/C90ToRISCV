#include <stdio.h>

float test();

int f(){
    return 200;
}

int main() {
    int x = 200;
    int *y = &x;
    printf("Test 1: %f\n", test());

    return 0;
}
