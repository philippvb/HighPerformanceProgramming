/**********************************************************************
 * Enumeration sort
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS	4
#define BLOCK_SIZE 100
#define len 100000

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

double indata[len], outdata[len];

void findrank(long j)
{
	int rank,i;
	rank=0;
	for (i=0;i<len;i++)
		if (indata[i]<indata[j]) rank++;
  outdata[rank]=indata[j];
}

// find multiple ranks for the blocks
void *findmultipleranks(void* arg){
  long start = (long) arg;
  for(int i=0; i<BLOCK_SIZE; i++){
    findrank(start + i);
  }
  return NULL;
}


int main(int argc, char *argv[]) {
	
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

  // Generate random numbers
  for (int i=0;i<len;i++){
      indata[i]=drand48();
      outdata[i]=-1.0;
  }
  printf("Finished generation\n");

  // Enumeration sort
  double startTime = get_wall_seconds();
  long start;
  for(long j=0; j<len; j+=BLOCK_SIZE*NUM_THREADS){
    for(long k=0; k<NUM_THREADS; k++){
      pthread_create(&threads[k], &attr, findmultipleranks, (void *) j + BLOCK_SIZE*k); }
    
    for(int k=0; k<NUM_THREADS; k++) 
      pthread_join(threads[k], NULL);
  }

  double timeTaken = get_wall_seconds() - startTime;
  printf("Time: %f  NUM_THREADS: %d\n", timeTaken, NUM_THREADS);

  // Check results, -1 implies data same as the previous element
    for (int i=0; i<len-1; i++)
      if (outdata[i]>outdata[i+1] && outdata[i+1]>-1)
	printf("ERROR: %f,%f\n", outdata[i],outdata[i+1]);

  return 0;
}
