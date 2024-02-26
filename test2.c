#include <stdio.h>

int test1() {
    int x = 3;

    do {
        x++;
        continue;
    } while (x < 4);

    return x;
}

int main()
{
    printf("%d\n", test1());
}
