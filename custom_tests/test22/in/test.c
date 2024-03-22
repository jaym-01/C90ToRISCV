typedef int_t *pint_t;

int_t test(int_t y)
{
    pint_t p;
    int_t x;
    x=y;
    p=&x;
    return 1+*p;
}
