/**********************************************************************
 * This program calculates pi using C
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

const int intervals = 500000000;
double dx  = 1.0/intervals;

// computes the integral of the interval (given as input [start, end]) and returns a pointer to its sum
void* compute_interval(void* d){
  int* data = (int *) d;
  double* thread_sum_p = malloc(sizeof(double));
  double x;
  for(int i=data[0]; i<data[1]; i++){
    x = dx*(i - 0.5);
    *thread_sum_p += dx*4.0/(1.0 + x*x);
  }
  return thread_sum_p;
}

int main(int argc, char *argv[]) {

  double sum = 0.0;
  pthread_t threads[NUM_THREADS];
  const int block_size = intervals/NUM_THREADS;
  int data[2*NUM_THREADS];
  for(int i=0; i<NUM_THREADS; i++){
    // start and end point for computation fo each thread
    data[i] = block_size*i;
    data[i+1] = block_size*(i+1);
    pthread_create(&threads[i], NULL, compute_interval, data+i);
  }

  // join and sum up the results
  for(int i=0; i<NUM_THREADS; i++){
    void* intermediate;
    pthread_join(threads[i], &intermediate);
    sum+= ((double * ) intermediate)[0];
    free((double * ) intermediate);
  }

  printf("PI is approx. %.16f\n",  sum);

  return 0;
}
