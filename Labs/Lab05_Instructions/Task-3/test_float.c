#include <stdio.h>
#include <math.h>

int main(){
    float i = 10.1;
    while(i != INFINITY){
        i *= 100;
        printf("%f\n", i);
    }
    printf("Adding to inf results in: %f\n", i + 100);
    printf("The theoretical max for the exponent is %f\n", pow(2, 127));
    float mynan = sqrt(-1);
    printf("Producing a NAN with sqrt: %f\n", mynan);
    printf("Adding to nan results in: %f\n", mynan + 100);

    float epsilon = 0.1;
    float eps_1;
    while(1){
        eps_1 = epsilon + 1;
        if (! (eps_1 > 1)) break;
        epsilon *= 0.5;
    }
    printf("The samllest epsilon for float is: %.20f", epsilon);


    return 0;
}