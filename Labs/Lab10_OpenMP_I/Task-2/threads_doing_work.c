#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void do_some_work() {
  printf("Now I am going to do some work...\n");
  long int i;
  long int j = 0;
  for(i = 0; i < 3000000000; i++) {
    j += 3;
  }
  printf("Work done! My result j = %ld\n", j);
}

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Please enter a number of threads\n");
    return -1;
  }
  int n_threads = atoi(argv[1]);
  double start = omp_get_wtime();
#pragma omp parallel num_threads(n_threads)
  {
    do_some_work();
  }
  
  printf("Exectution took: %fs\n", omp_get_wtime() - start);

  return 0;
}
