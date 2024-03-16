// enum Vals{
//     val1 = -100, val2, val3, val4 = 100, val5
// };

// int test(){
//     int x = 101;
// }

// enum Vals test2(enum Vals test){
//     return test;
// }

int test(int lo, int hi, int val){
    // while(lo+1 < hi){
    //     int mid=(lo+hi)>>1;
    //     int sqr=mid*mid;
    //     if(sqr <= val){
    //         lo=mid;
    //     }else{
    //         hi=mid;
    //     }
    // }
    // if( lo*lo < val ) {
    //     return hi;
    // }else{
    //     return lo;
    // }
    int res = (lo+hi)>>1;return res;
}
