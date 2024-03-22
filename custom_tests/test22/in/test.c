// typedef int t_int;
// // typedef t_int test1;

// int test(){
//     t_int a = 1;
//     // return a;
//     return a;
// }

typedef int int_t;

typedef int_t *pint_t;

int_t test(int_t y)
{
    pint_t p;
    int_t x;
    x=y;
    p=&x;
    return 1+*p;
}
