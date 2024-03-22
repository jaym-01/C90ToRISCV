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

int f(int x, int y)
{
    int acc=0;
    // if(x < 0){
    //     return -f(-x, y);
    // }

    // while(x > 0){
    //     acc += y;
    //     x--;
    // }
    acc += y;
    return acc;
}
