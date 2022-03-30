#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define THREADED 1

double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}


// Checks if a number is prime
int isprime(int i){
    for(int j=2; j<i; j++){
        if(i%j==0){
            return 0;
        }
    }
    return 1;
}

// Count all primes in a range, see main for args
void* count_primes(void* values){
    int* int_values = (int*) values;
    for(int i=int_values[0]; i<=int_values[1]; i++){
        int_values[2] += isprime(i);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) return -1;
    
    // up to which number to count the primes
    const int N = atoi(argv[1]);
    if(N<1){
        printf("Please enter a positive number\n");
        return -1;
    }
    else if(N<3){
        printf("There are %d primes from 0 to %d\n", N, N);
    }
    else{

        #if THREADED
        int thread_1_data[3], thread_2_data[3]; 
        // each thread gets its data as [lower, upper, result]
        thread_1_data[0] = 1;
        thread_1_data[1] = N*2/3; // 2/3 split
        thread_1_data[2] = 0;
        thread_2_data[0] = N*2/3+1;
        thread_2_data[1] = N;
        thread_2_data[2] = 0;
        pthread_t thread1, thread2;
        double start = get_wall_seconds();
        pthread_create(&thread1, NULL, count_primes, &thread_1_data);
        pthread_create(&thread2, NULL, count_primes, &thread_2_data);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        double end = get_wall_seconds();
        printf("The execution took %f seconds\n", end-start);
        printf("There are %d primes from 0 to %d\n", thread_2_data[2] + thread_1_data[2], N);

        #else
        // inital serial code
        int total_primes = 2;
        double start = get_wall_seconds();
        for(int i=3; i<=N; i++){
            total_primes += isprime(i);
        }
        double end = get_wall_seconds();
        printf("There are %d primes from 0 to %d\n", total_primes, N);
        printf("The execution took %f seconds\n", end-start);
        #endif
    }
    return 0;
}
