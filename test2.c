#include <stdio.h>

int test1() {
    int x = 3;
    int y = 2;

    if (x < 3) {
        x = 3;
    }
    else {
        if (y < 3) {
            x = 4;
        } else {
            x = 5;
        }
    }

    return x;
}

int main()
{
    printf("%d\n", test1());
}
