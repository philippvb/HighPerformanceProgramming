#include <stdio.h>
#include <stdlib.h>

int main(){
    int a, b, c;
    printf("Enter 3 integers: \n");
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);

    if ((abs(a) > abs(b)) && (abs(a) > abs(c)))
    printf("%d", a);
    else if (abs(b) > abs(c))
    printf("%d", b);
    else
    printf("%d", c);  
    printf("\n");
    return 0;
}