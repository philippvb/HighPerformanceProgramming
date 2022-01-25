#include <stdio.h>

int main(){
    int a, b;
    printf("Enter integer a: \n");
    scanf("%d", &a);
    printf("Enter integer b: \n");
    scanf("%d", &b);
    for(int row=0; row < a; row++){
        for(int col=0; col<b; col++){
            if((row==0) || (row == a-1) || (col==0) || (col==b-1)){
                printf("*");
            }
            else{
                printf(".");
            }
        }
        printf("\n");
    }
    return 0;
}