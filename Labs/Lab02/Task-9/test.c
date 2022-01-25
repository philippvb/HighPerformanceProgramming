#include <stdio.h>


int main(){
    int a = 0;
    int *p;
    p = &a;
    int offset = 5;
    printf("%d\n", *(p+offset));
    printf("%d\n", p[offset]);
return 0;
}