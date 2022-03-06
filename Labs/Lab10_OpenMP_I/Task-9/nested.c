#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long int sum = 0;
int N = 100000;

void the_thread_func() {

  for(int i = 1; i <= N; ++i) {
    #pragma omp critical
    sum += 1;
  }

}

int main(int argc, char **argv) {

  if(argc != 2) {printf("Usage: %s N\n", argv[0]); return -1; }
  int nThreads = atoi(argv[1]);

  printf("This is the main() function before the parallel block, nThreads = %d\n", nThreads);
  omp_set_nested(1);
#pragma omp parallel num_threads(nThreads)
{
  printf("N threads in outer %d\n", omp_get_num_threads());
  #pragma omp parallel num_threads(nThreads)
  { 
    printf("N threads in inner %d\n", omp_get_num_threads());
    the_thread_func();
  }
}
  
  printf("This is the main() function after the parallel block\n");

  printf("sum = %ld\n", sum);

  return 0;
}
