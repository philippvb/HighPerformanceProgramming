#include <stdio.h>
int main(){
    int a, b;
    printf("Enter integer a: \n");
    scanf("%d", &a);
    printf("Enter integer b: \n");
    scanf("%d", &b);
    if ((a%2==0) && (b%2==0)){
        printf("%d\n", a+b);
    }
    else{
        printf("%d\n", a*b);
    }
    return 0;
}