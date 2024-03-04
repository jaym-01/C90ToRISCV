#include <stdio.h>

// float test1(float x, float y, float z);
// int test1();
float f(float x, int n);

int main() {
    // printf("Test 1: %f\n", test1(1.5, 2.5, 3.5));
    // printf("Test 1: %d\n", test1());
    printf("%f\n", f(5.5, 2));

    return 0;
}
