#include <stdio.h>

char test(char *x, char c);

int f(){
    return 200;
}

char *search(char *x, char c);

int main() {
    int x = 200;
    int *y = &x;
    printf("Test 1: %c\n", search("abcdef", 'c'));

    return 0;
}
