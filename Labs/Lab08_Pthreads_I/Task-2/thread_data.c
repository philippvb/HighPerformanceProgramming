#include <stdio.h>
#include <pthread.h>

void* the_thread_func(void* arg) {
  double* arr = (double * ) arg;
  printf("The values are %f %f %f\n", arr[0], arr[1], arr[2]);
  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  double data_for_thread[3];
  data_for_thread[0] = 5.7;
  data_for_thread[1] = 9.2;
  data_for_thread[2] = 1.6;

  double data_for_thread_2[3];
  data_for_thread_2[0] = 1.0;
  data_for_thread_2[1] = 2.0;
  data_for_thread_2[2] = 3.0;

  /* Start thread. */
  pthread_t thread, thread2;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, data_for_thread);
  pthread_create(&thread2, NULL, the_thread_func, data_for_thread_2);

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);
  pthread_join(thread2, NULL);

  return 0;
}
