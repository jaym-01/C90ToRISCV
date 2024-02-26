#include <stdio.h>

int test1() {
    int x = 3;

    do {
        x++;
    } while (x < 10);

    return x;
}

int main()
{
    printf("%d\n", test1());
}
