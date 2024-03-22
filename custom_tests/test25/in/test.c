
int test1(){
    float x = 2.5, y = 3.5;
    double z = 4.5, h = 4;

    return (x < y) == 1;
}

char test2(){
    char a = '\\';
    return a;
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
