#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long int Fib(int n, int n_threads) {
  if(n < 2) 
    return n;
  long int i1, i2;
  if (n_threads > 2)
    #pragma omp parallel num_threads(2)
      if (omp_get_thread_num() == 1)
        i1 = Fib(n-1, n_threads/2);
      else
        i2 = Fib(n-2, n_threads/2);
  else{
    i1 = Fib(n-1, 0);
    i2 = Fib(n-2, 0);
  }
  return i1 + i2;
}

int main(int argc, char** argv) {
  omp_set_nested(1);
  if(argc != 3) {
    printf("Please give two input argr: n, n_threads\n");
    return -1;
  }
  int n = atoi(argv[1]);
  int n_threads = atof(argv[2]);
  printf("n = %d\n", n);
  long int Fib_of_n = Fib(n, n_threads);
  printf("Fib(%d) = %ld\n", n, Fib_of_n);
  return 0;
}
