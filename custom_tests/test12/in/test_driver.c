#include <stdio.h>

unsigned test(unsigned new_val[]);

int main() {
    unsigned vals[] = {-1, 2, 3};
    printf("Test 1: %u\n", test(vals));

    return 0;
}
