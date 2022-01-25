#include <stdio.h>

int main(){
    int max = 11;

    printf("With for loop: ");
    for(int i=0; i < max; i++){
        printf("%d ",i);
    }

    printf("\nWith do-while loop: ");
    int i = 0;
    do{
        printf("%d ",i);
        i++;
    }while (i< max);

    printf("\nWith while loop: ");
    i = 0;
    while(i< max){
        printf("%d ",i);
        i++;
    }
    return 0;
}