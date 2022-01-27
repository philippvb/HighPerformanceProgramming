#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIDE 1

int main(int argc, char *argv[]){   
    if (argc != 2){
    printf("Please enter the size of the triangle.\n");
    return -1;
    }

    int n;
    sscanf(argv[1], "%d", &n);
    int *lastrowarr = malloc(n*(sizeof(int)));
    lastrowarr[0] = 1;
    lastrowarr[1] = 1;
    int *thisrowarr = malloc(n*(sizeof(int)));
    int *placeholder;

    // initial string
    printf("1\n1\t1\n");
    // remaining rows
    for(int i=2; i<n; i++){
        thisrowarr[0] = SIDE;
        printf("%d\t", SIDE);
        for(int j=1; j<i; j++){
            thisrowarr[j] = lastrowarr[j-1] + lastrowarr[j];
            printf("%d\t", thisrowarr[j]);
        }
        thisrowarr[i] = SIDE;
        placeholder = lastrowarr;
        lastrowarr = thisrowarr;
        thisrowarr = placeholder;
        printf("%d\n", SIDE);
    } 
    // free memory
    free(lastrowarr);
    free(thisrowarr);
    return 0; 
}