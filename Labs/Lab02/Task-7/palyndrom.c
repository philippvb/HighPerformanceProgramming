#include <stdio.h>
#include <math.h>

int main(){
    int num;
    int length = 1;
    scanf("%d", &num);
    while((num/((int) pow(10, length)) > 0)){
        length++;
    }
    int arr[length];
    for(int i = 0; i<length; i++){
        arr[i] = num%10;
        num /= 10;
    }

    int ispalyndrom=1;
    for(int i = 0; i < length/2; i++){
        if(arr[i] != arr[length-i-1]){
            ispalyndrom = 0;
            printf("The number is no palyndrom");
            break;
        }
    }
    if (ispalyndrom)
    printf("The number is palyndrom");
    return 0;
}