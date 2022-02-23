#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// typedef struct threaddata
// {
//     int level;
//     int id;
// } threaddata;

void* thread_hello(void* data){
    printf("Hello from Thread\n");
    return NULL;
}

void* create_threads(void * data){
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_hello, NULL);
    pthread_create(&thread2, NULL, thread_hello, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, create_threads, NULL);
    pthread_create(&thread2, NULL, create_threads, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
