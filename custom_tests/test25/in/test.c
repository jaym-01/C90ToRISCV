
int test1(){
    // float x = 2.5, y = 3.5;
    // double z = 4.5, h = 4;

    // return (x < y) == 1;
    char x = 5, *y = &x;
    return sizeof(*y);
}

char test2(){
    struct test{
        char a;
        int b;
    };

    struct test t;
    t.a = 'a';
    return t.a;
}

int test3(){
    int x = 5, y = 10;
    int *z = &x, **h = &z, *i = &y;
    *h = i;
    return *z;
}

int a1[] = {1, 2, 3, 4, 5};
int *p1 = a1;

int test4(){
    return *(p1 + 2);
}

float* test5(){
    float x = 5.5, *y = &x;
    return y;
}

char *test6(){
    char x = 'a', *y = &x;
    return y;
}
