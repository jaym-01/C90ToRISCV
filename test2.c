#include <stdio.h>
int main() {
    int i = 0;


    // Loops
    loop_start:
        printf("Hello, World!\n");
        if (i == 0) {
            i++;
            goto loop_start;
        }

    ;

    do {
        printf("%d\n", i);
        i++;
    } while (i < 10);
}
