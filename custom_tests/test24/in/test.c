// // int test(){
// //     int x = 5, h = 100;
// //     int *y = &x;
// //     int **z = &y;
// //     **z = h;
// //     return **z;
// // }

// int f()
// {
//     if(0){
//         return 11;
//     }else{
//         return 10;
//     }
// }

typedef int test111;

int test1(){
    // float x = 2.5, y = 3.5;
    // double z = 4.5, h = 4;

    // return (x < y && z < h) || (x > y && z > h);

    return sizeof(test111);
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
