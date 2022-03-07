#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}

int isprime(int i){
    for(int j=2; j<i; j++){
        if(i%j==0){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) return -1;
    
    const int N = atoi(argv[1]);
    int n_primes = 0;
    const int n_threads = atoi(argv[2]);
    if(N<1){
        printf("Please enter a positive number\n");
        return -1;
    }
    else if(N<3){
        printf("There are %d primes from 0 to %d\n", N, N);
    }
    else{
        double start = get_wall_seconds();
        #pragma omp parallel for num_threads(n_threads) reduction(+:n_primes) schedule(guided)
        for(int i=0; i<N; i++){
            n_primes += isprime(i);
        }
        printf("Exectution took %fs\n", get_wall_seconds() - start);
        printf("There are %d primes from 0 to %d\n", n_primes, N);
    }
    return 0;
}
