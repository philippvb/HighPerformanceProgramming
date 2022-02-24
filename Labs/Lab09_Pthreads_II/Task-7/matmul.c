#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 25

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

double **A,**B,**C;
int n;


void matmul(int row, int col){
  double sum=0.0;
  for(int i=0; i<n; i++){
    sum+=A[row][i] * B[i][col];
  }
  C[row][col] = sum;
}
typedef struct threaddata
{
  int i_start;
  int j_start;
  int i_end;
  int j_end;
} threaddata;

void* matmul_loop(void *args){
  threaddata* data = (threaddata*) args;
  for(int i=data->i_start; i<data->i_end; i++){
    for(int j=data->j_start; j<data->j_end; j++){
      matmul(i, j);
    }
  }
  return NULL;
}



int main(int argc, char *argv[]) {
  int i, j, k;

  if(argc != 2) {
    printf("Please give one argument: the matrix size n\n");
    return -1;
  }

  n = atoi(argv[1]);
  
  //Allocate and fill matrices
  A = (double **)malloc(n*sizeof(double *));
  B = (double **)malloc(n*sizeof(double *));
  C = (double **)malloc(n*sizeof(double *));
  for(i=0;i<n;i++){
    A[i] = (double *)malloc(n*sizeof(double));
    B[i] = (double *)malloc(n*sizeof(double));
    C[i] = (double *)malloc(n*sizeof(double));
  }

  for (i = 0; i<n; i++)
    for(j=0;j<n;j++){
      A[i][j] = rand() % 5 + 1;
      B[i][j] = rand() % 5 + 1;
      C[i][j] = 0.0;
    }

  printf("Doing matrix-matrix multiplication...\n");
  double startTime = get_wall_seconds();

  pthread_t threads[NUM_THREADS];
  threaddata threads_data[NUM_THREADS];
  int block_size = n/sqrt(NUM_THREADS);
  int cur_thread = 0;
  for(int i=0; i<n; i+=block_size){
    for(int j=0; j<n; j+=block_size){
      threads_data[cur_thread].i_start = i;
      threads_data[cur_thread].i_end = i + block_size;
      threads_data[cur_thread].j_start = j;
      threads_data[cur_thread].j_end = j + block_size;
      pthread_create(&threads[cur_thread], NULL, matmul_loop, (void *) &threads_data[cur_thread]);
      cur_thread++;
    }
  }

  for(int t = 0; t<NUM_THREADS; t++){
    pthread_join(threads[t], NULL);
  }

  double timeTaken = get_wall_seconds() - startTime;
  printf("Elapsed time: %f wall seconds\n", timeTaken);

  // Correctness check (let this part remain serial)
  printf("Verifying result correctness for a few result matrix elements...\n");
  int nElementsToVerify = 5*n;
  double max_abs_diff = 0;
  for(int el = 0; el < nElementsToVerify; el++) {
    i = rand() % n;
    j = rand() % n;
    double Cij = 0;
    for(k = 0; k < n; k++)
      Cij += A[i][k] * B[k][j];
    double abs_diff = fabs(C[i][j] - Cij);
    if(abs_diff > max_abs_diff)
      max_abs_diff = abs_diff;
  }
  printf("max_abs_diff = %g\n", max_abs_diff);
  if(max_abs_diff > 1e-10) {
    for(i = 0; i < 10; i++)
      printf("ERROR: TOO LARGE DIFF. SOMETHING IS WRONG.\n");
    return -1;
  }
  printf("OK -- result seems correct!\n");
  
  // Free memory
  for(i=0;i<n;i++){
    free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);

  return 0;
}
