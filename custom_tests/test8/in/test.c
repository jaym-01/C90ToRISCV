// int test1(){
//     float x = 5.55 + 1.5;
//     return 0;
// }

// float glbl_x = 1.5 + 10.5;

// int test1(){
//     float x = 5.55;
//     float y = 10.5;
//     float test = x + y + glbl_x;
//     return 0;
// }

// int test2(float x){
//     float a = x + 1.0;
//     return 0;
// }

// // int test1(int x){
// //     return test2(x, x + 1, x + 2);
// // }

// int test1(){
//     test2(5.5);
//     return 0;
// }

// float test1(float x, float y, float z)
// {
//     return x*y+z;
// }

// int test1(){
//     return 5;
// }


float f(float x, int n)
{
    float acc=1.0f;
    int i=0;
    while(i<n){
        i++;
        acc=acc*x;
    }
    return acc;
}
