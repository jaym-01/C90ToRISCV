// Test 1: that x decrements after the assignment to z.
int test1a() {
    int x = 3;
    int z = x--;
    return x; // 2
}

int test1b() {
    int x = 3;
    int z = x--;
    return z; // 3
}

int test2a() {
    int x = 3;
    int y = --x;

    // return y;
    return x; // 2
}

int test2b() {
    int x = 3;
    int y = ++x;

    // return y;
    return y; // 4
}

int test2c(){
    int x = 3;
    int y = -x;

    return x; // 2
}

int test2c()
{
    int x = 3;
    int y = ~x;

    return x; // Should be -x - 1
}

int test2d()
{
    int x = 3;
    return !x; // 0 if x != 0, 1 if x == 0
}

int test2e()
{
    int x = 3;
    int* y = &x;

    return *y; // 2
}
