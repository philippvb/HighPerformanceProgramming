#include <stdio.h>

int main(){
    int max = 100;
    int decrease = 4;
    int min = 0;

    printf("With for loop: ");
    for(int i=max; i >= min; i-=decrease){
        printf("%d ",i);
    }

    printf("\nWith do-while loop: ");
    int i = max;
    do{
        printf("%d ",i);
        i-=decrease;
    }while (i >= min);

    printf("\nWith while loop: ");
    i = max;
    while(i >= min){
        printf("%d ",i);
        i-=4;
    }
    return 0;
}