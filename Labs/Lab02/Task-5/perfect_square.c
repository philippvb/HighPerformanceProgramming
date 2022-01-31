#include <stdio.h>
#include <math.h>

int main(){
    int n;
    printf("Please enter your number:");
    scanf("%d", &n);
    double root;
    root = sqrt(n);
    if(!fmod(root, 1))
    printf("Your number is a perfect square");
    else
    printf("Your number is no perfect square");
    return 0;
}