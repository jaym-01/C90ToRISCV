#include <stdio.h>

float test(float x[]);
double test2(double x[]);
char test3();
int test4(int x[]);

int main() {
    int x[] = {552, 2, 3, 4, 5};
    float y[] = {0.277, 2.1, 3.2, 4.5, 5.43};
    double z[] = {200.3, 2.1, 3, 4, 5};
    printf("Test 1: %f\n", test(y));
    printf("Test 2: %lf\n", test2(z));
    printf("Test 3: %c\n", test3());
    printf("Test 4: %d\n", test4(x));

    return 0;
}
