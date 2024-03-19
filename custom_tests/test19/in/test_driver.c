#include <stdio.h>

char *test();

int main() {
    int x = 200;
    int *y = &x;
    printf("Test 1: %s\n", test());
    // printf("Test 2: %c\n", test("abcdef", 'a'));

    return 0;
}
