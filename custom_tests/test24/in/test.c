int test(){
    int x = 5, h = 100;
    int *y = &x;
    int **z = &y;
    **z = h;
    return **z;
}
