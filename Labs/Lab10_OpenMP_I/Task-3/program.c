#include <stdio.h>
#include <omp.h>

void thread_func() {
  printf("Thread_id: %d\n", omp_get_thread_num());
  printf("This is inside thread_func()!\n");
}

int main(int argc, char** argv) {

#pragma omp parallel num_threads(8)
  {
    thread_func();
  }
  printf("N_threads: %d\n", omp_get_num_threads());
  printf("Max_Threads: %d\n", omp_get_max_threads());

  return 0;
}
