#include <stdio.h>

enum Vals{
    val1 = -100, val2, val3, val4 = 100, val5
};

int test();
enum Vals test2(enum Vals test);

int main() {
    printf("Test 1: %d\n", test());
    printf("Test 1: %d\n", test2(val5));

    return 0;
}
