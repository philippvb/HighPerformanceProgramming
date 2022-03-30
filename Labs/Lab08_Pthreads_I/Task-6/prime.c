#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// the data for each thead as a structure
typedef struct threaddata
{
    int start;
    int end;
    int sum;
} threaddata;

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

void* count_primes(void* values){
    threaddata* data = (threaddata*) values;
    for(int i=data->start; i<=data->end; i++){
        data->sum += isprime(i);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) return -1;
    const int M = atoi(argv[1]);        
    const int N = atoi(argv[2]);
    if(N<1){
        printf("Please enter a positive number\n");
        return -1;
    }
    else if(N<3){
        printf("There are %d primes from 0 to %d\n", N, N);
    }
    else if(N%M != 0){
        printf("Please enter a number N that is dividable by the number of threads M\n");
        return -1;
    }
    else{
        pthread_t threads[M];
        threaddata data[M];
        double start = get_wall_seconds();
        int block_size = N/M;
        // create all the threads
        for(int i=0; i<M; i++){
            data[i].start=i*block_size+1;
            data[i].end=(i+1)*block_size;
            data[i].sum=0;
            pthread_create(threads + i, NULL, count_primes, data + i);
        }

        // join and count all results
        int n_primes=0;
        for(int i=0; i<M; i++){
            pthread_join(threads[i], NULL);
            n_primes += data[i].sum;
        }
        double end = get_wall_seconds();
        printf("The execution took %f seconds\n", end-start);
        printf("There are %d primes from 0 to %d\n", n_primes, N);

    }
    return 0;
}
