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

int count_primes(int start, int end){
    int sum = 0;
        for(int i=start; i<end; i++){
        sum += isprime(i);
    }
    return sum;
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
        double block_size;
        #ifdef _OPENMP
        block_size = (double) N/ (double) n_threads;
        #else 
        block_size = N;
        #endif

        double start = get_wall_seconds();
        #pragma omp parallel num_threads(n_threads)
        {   
            double id;
            #ifdef _OPENMP
            id = omp_get_thread_num();
            #else
            id = 0;
            #endif
            n_primes += count_primes((int) (block_size * id), (int) (block_size * (id + 1)));
        }
        printf("Exectution took %fs\n", get_wall_seconds() - start);
        printf("There are %d primes from 0 to %d\n", n_primes, N);
    }
    return 0;
}
