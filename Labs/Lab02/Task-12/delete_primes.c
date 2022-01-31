#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isprime(int num){
    if(num<1) return 0;
    if((num==1) || (num==2)) return 1;
    for(int i=2; i<=(int)sqrt(num); i++){
        if(num%i==0) return 0;
    }
    return 1;
}


int main()
{
    int n;
    int* arr;
    printf("Enter a size: ");
    scanf("%d", &n);
    arr = malloc(n * sizeof(int));
    int *arr_no_prime = malloc(n * sizeof(int));
    int num_no_primes = 0;
    printf("Enter array values: ");
    for(int i = 0; i< n; i++){
        scanf("%d", arr+i);
    }
    for(int i=0; i<n; i++){
        if(!isprime(arr[i])){
            arr_no_prime[num_no_primes] = arr[i];
            num_no_primes++;
        }
    }
    arr_no_prime = realloc(arr_no_prime, num_no_primes*sizeof(int));
    for(int i=0; i<num_no_primes; i++){
        printf("%d ", arr_no_prime[i]);
    }
    free(arr);
    free(arr_no_prime);
return 0; }