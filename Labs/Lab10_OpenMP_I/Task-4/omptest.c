#include <stdio.h>
#include <omp.h>

typedef struct DataForThread {
  double A;
  double B;
  int x;
  int y;
  double result;
} DataForThread_t;

double thread_func(DataForThread_t* p) {
  printf("This is inside thread_func()!\n");
  printf("%f, %f, %d, %d\n", p->A, p->B, p->x, p->y);
  return p->A + p->B;
}

int main(int argc, char** argv) {

  const int nThreads = 2;
  DataForThread_t arr[nThreads];
  /* Prepare data for thread 0. */
  arr[0].A = 3.7;
  arr[0].B = 1.2;
  arr[0].x = 88;
  arr[0].y = 77;
  /* Prepare data for thread 1. */
  arr[1].A = 2.2;
  arr[1].B = 8.8;
  arr[1].x = 444;
  arr[1].y = 555;

#pragma omp parallel num_threads(nThreads)
  {
    /* OK, now we are inside the parallel block, so this is executed
       by several threads. Get ID of current thread. */
    int id = omp_get_thread_num();
    /* Call thread_func and give it a pointer to arr[id] as input. */
    arr[id].result = thread_func(&arr[id]);
    printf("Result from thread %d is: %f\n", id, arr[id].result);
  }

  return 0;
}
