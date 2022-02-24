#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* the_thread_func(void* arg) {
  int* p = malloc(sizeof(int));
  p[0] = 1234;
  pthread_exit(p);
  // return p;
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  if(pthread_create(&thread, NULL, the_thread_func, NULL) != 0) {
    printf("ERROR: pthread_create failed.\n");
    return -1;
  }

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
  void* thread_return;

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  if(pthread_join(thread, &thread_return) != 0) {
    printf("ERROR: pthread_join failed.\n");
    return -1;
  }
  printf("Data from thread malloc %d\n",((int *) thread_return)[0]);
  free((int *) thread_return);

  return 0;
}
