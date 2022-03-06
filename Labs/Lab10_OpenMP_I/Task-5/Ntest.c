#include <stdio.h>
#include <omp.h>

const long int NA = 700000000;
const long int NB = 100000000;

long int thread_func_A() {
  long int i;
  long int sum = 0;
  for(i = 0; i < NA; i++)
    sum += 7;
  printf("Finished thread A\n");
  return sum;
}

long int thread_func_B() {
  long int i;
  long int sum = 0;
  for(i = 0; i < NB; i++)
    sum += 7;
  printf("Finished thread B\n");
  return sum;
}

int main() {
  printf("This is the main() function starting.\n");

  long int result_A;
  long int result_B;
  double start = omp_get_wtime();
#pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    if(id == 0)
      result_A = thread_func_A();
    else
      result_B = thread_func_B();
  }
  printf("Execution took %fs\n", omp_get_wtime() - start);

  printf("This is the main() function after the parallel block.\n");

  printf("result_A : %ld\n", result_A);
  printf("result_B : %ld\n", result_B);
  long int totalSum = result_A + result_B;
  printf("totalSum : %ld\n", totalSum);

  return 0;
}
