// char test(){
//     // char s[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
//     char *s = "abcdefghij";
//     s = "hello";
//     *s = 'p';
//     // s[0] = 'h';
//     return *(s + 1);
// }

char *search(char *x, char c){
    while(*x){
        if(*x==c){
            return x;
        }
        x=x+1;
    }
    return 0;
}


// int search(char *x, char c) {
//     return *(x + 2) == c;
// }
