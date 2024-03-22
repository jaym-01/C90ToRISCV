// char test(){
//     char c = 'a';
//     char *x = &c;
//     char **y = &x;
//     char ***z = &y;
//     return ***z;
// }

// float test(){
//     float c = 2.5;
//     float *x = &c;
//     float **y = &x;
//     float ***z = &y;
//     return ***z;
// }

// double test(){
//     double c = 2.54555  ;
//     double *x = &c;
//     double **y = &x;
//     double ***z = &y;
//     return ***z;
// }

// char test(){
//     char c = 'a';
//     char *x = &c;
//     x[0] = 'b';
//     return x[0];
// }


float test(){
    float c[] = {2.5, 3.5};
    float *x = c;
    x[0] = 100.5;
    return *(x + 1);
}
