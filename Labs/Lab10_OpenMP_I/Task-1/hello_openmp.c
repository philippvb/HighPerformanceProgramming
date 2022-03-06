#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv) {
  omp_set_num_threads(6); // we set n_threads to 6
#pragma omp parallel num_threads(3) //however this overwrites since higher priority
  {
    printf("Bonjour!\n");
  }

  return 0;
}
