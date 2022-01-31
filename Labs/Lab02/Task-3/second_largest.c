#include <stdio.h>
#include <stdlib.h>


int compare(const void *a_p, const void *b_p){
    int a = abs(*((int * )a_p ));
    int b = abs(*((int * )b_p ));
    return -1*(a < b) + 0 *(a==b) + 1 * (a>b);
}

int main(){
    int numbers[3];
    printf("Enter 3 integers: \n");
    scanf("%d", numbers);
    scanf("%d", numbers+1);
    scanf("%d", numbers+2);
    qsort(numbers, 3, sizeof(int), compare);
    printf("%d", numbers[1]);
}