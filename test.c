int f(int n)
{
    if (n == 0) {
        return 0;
    }
    return n + f(n - 1);
}

int test1() {
    return f(2);
}
