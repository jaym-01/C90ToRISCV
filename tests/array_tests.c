int test1() {
    int y = 0;

    // Test array declaration
    int x[3] = {1, 2, 3};
    int y, z;

    // Test array elem assignment
    x[0] = 4 * 3;

    // Test array postfix, prefix operation
    y = ++x[0];
    z = x[0];

    // Test array elem read
    return y;
}

// Test 2: Global variable arrays
int y[] = {1, 3, 5};
int test2() {

    y[1] = 5;

    ++y[1];
    y[1]++;
    return ++y[1];

}
