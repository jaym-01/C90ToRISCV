#include <stdio.h>
// int align_to_multiple_of_4(int offset)
// {
//     int remainder = offset % 4;
//     if (remainder == 0)
//     {
//         return offset;
//     }
//     else
//     {
//         return offset + (4 - remainder);
//     }
// }

int test2(int x) {
    x = 5;

    printf("x in func = %d\n", x);
}

int main() {
    int x = 1;
    test2(x);
    printf("x in main = %d\n", x);
}
