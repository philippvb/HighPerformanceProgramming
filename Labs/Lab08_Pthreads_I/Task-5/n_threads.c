#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

void* print_id(void* id){
    printf("Thread %d\n", (int) id);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) return -1;
    
    const int N = atoi(argv[1]);
    if(N<1){
        printf("Please enter a positive number\n");
        return -1;
    }
    pthread_t threads[N];
    for(int i=0; i<N; i++){
        // we need to cast i to void pointer, since we want to pass by value (this is safe on most systems because the size of a pointer is larger than int)
        pthread_create(threads+i, NULL, print_id, (void*) i);
    }
    for(int i=0; i<N; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}
