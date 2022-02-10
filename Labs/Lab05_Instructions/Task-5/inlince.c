#include <math.h>
#include <stdlib.h>
// static inline void somecomputation(int i);
void somecomputation(int i){
    double *arr;
    arr = malloc(100 * sizeof(double));
    for(int i = 0; i<100; i++){
        arr[i] = rand();
    }
    free(arr);
}

void nonleaffunction(int i){
    for(int j = 0; j < 10; j++){
        somecomputation(i);
    }   
}

void nonleaffunction2(int i){
    nonleaffunction(i);
    for(int j = 0; j < 10; j++){
        somecomputation(i);
    }   
}



int main(){
    for(int i = 0; i < 10000; i++){
        // somecomputation(i);
        for(int j=0; j < 10; j++){
            nonleaffunction(i*j);
        }
    }
}