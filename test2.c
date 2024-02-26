#include <stdio.h>

int test1(int n)
{
    if (n == 0)
    {
        return 0;
    }
    return n + test1(n - 1);
}

int main()
{
    printf("%d\n", test1(5));
}
