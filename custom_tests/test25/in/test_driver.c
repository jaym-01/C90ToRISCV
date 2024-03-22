#include <stdio.h>

int test1();
char test2();
int test3();
int test4();
float* test5();
char *test6();

int main() {
    printf("Test 1: %d\n", test1());
    printf("Test 2: %d\n", test2());
    printf("Test 3: %d\n", test3());
    printf("Test 4: %d\n", test4());
    printf("Test 5: %f\n", *test5());
    printf("Test 6: %c\n", *test6());

    return 0;
}
