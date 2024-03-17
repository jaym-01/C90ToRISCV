// int test(){
//     int x = 5;
//     int *y = &x;
//     int z = *y;
//     return z;
// }

// int test(){
//     int x = 5;
//     int *y = &x;
//     *y = 100;
//     return *y;
// }

// int test() {
//     int x[] = {100, 200, 300}, *y, **z;

//     y = &x;
//     z = &y;

//     return **z;
// }

// int test(){
//     int x[] = {1, 2, 2,4};
//     int *y = x;
//     y[3] = 200;
//     return y[3];
// }

int test(){
    int x[] = {1, 2, 3};
    int *y = x;
    return *(y + 1);
}

// int test(){
//     int x[] = {500, 2, 3}, *y;
//     y = x;
//     return y[1];
// }


// enum Vals{
//     val1, val2, val3, val4
// };

// int test(){
//     enum Vals test1 = val4, *test2;
//     test2 = &test1;
//     return *test2;
// }
