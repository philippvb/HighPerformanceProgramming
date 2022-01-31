#include <stdio.h>
#include <math.h>

int main(){
    int dividend;
    printf("Enter dividend:");
    scanf("%d", &dividend);
    int divisor;
    printf("Enter divisor:");
    scanf("%d", &divisor);
    printf("Quotient: %d\n", dividend/divisor);
    printf("Remainder: %d\n", dividend%divisor);

}