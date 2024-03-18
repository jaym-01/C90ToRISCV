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

// int test(){
//     int x[] = {1, 2, 3};
//     int *y = x;
//     return *(y + 1);
// }

// int test(){
//     int x[] = {500, 2, 3}, *y;
//     y = x;
//     return y[1];
// }


// int array[] = {1,2,3,4,5};

// int test(){
//     int *y = array;
//     return *(y + 3);
// }

// int test(){
//     int array_local[] = {1,2,3,4,5};
//     int *y = &array_local[4];
//     return *y;
// }


// TODO:: Fix this
// int test(){
//     int y = 100;
//     int *x = (&y + 5) + 2;
//     return y;
// }

// enum Vals{
//     val1, val2, val3, val4
// };

// int test(){
//     enum Vals test1 = val4, *test2;
//     test2 = &test1;
//     return *test2;
// }





int x = 4;
int *y = &x;

int test(){
    return *(y + 1);
}


// int x[] = {1,2,3,4};
// int *y = x;

// int test(){
//     return y[0];
// }
