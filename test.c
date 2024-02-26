int f(int n)
{
    if (n == 0) {
        return 0;
    }
    return n + f(n - 1);
}

int test1() {
    int x = 3;
    int y = f(3);

    int z = f(8);
    int k = f(2);

    return x + y + z + k;
}
