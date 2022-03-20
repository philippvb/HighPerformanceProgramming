#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define USE_EXAMPLE 0


double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}
typedef struct givens{
  int i;
  int j;
  double c;
  double s;
} givens_t;

inline void matmul_givens_R(givens_t g, double* X, int m, int n){
    double* row_copy = malloc(2*n*sizeof(double));
    // copy the two rows in intermediate matrix
    for(int i=0; i<n; i++){
        row_copy[i] = X[g.i*n + i];
        row_copy[n+i] = X[g.j*n + i];
    }
    // iterate over all cols
    for(int k=0; k<n; k++){
        // X_ik = c * A_ik - s * A_jk
        X[g.i*n + k] = g.c * row_copy[k] - g.s * row_copy[n+k];

        // X_jk = s * A_ik + c * A_jk
        X[g.j*n + k ] = g.s * row_copy[k] + g.c * row_copy[n+k];        
    }
    free(row_copy);
}

inline void matmul_givens_Q(givens_t g, double* X, int n){
    // both matrices
    double* col_copy = malloc(2*n*sizeof(double));
    // copy the two cols in intermediate matrix
    for(int i=0; i<n; i++){
        col_copy[i] = X[g.i * n + i];
        col_copy[n+i] = X[g.j * n + i];
    }
    
    for(int k=0; k<n; k++){
        // X_ki = c * A_ki - s * A_kj
        X[g.i*n + k] = g.c * col_copy[k] - g.s * col_copy[n+k];

        // X_kj = s * A_ik + c * A_kj
        X[g.j*n + k] = g.s * col_copy[k] + g.c * col_copy[n+k];        
    }
    free(col_copy);
}

inline void compute_givens_factors(double a, double b, givens_t* G){
    double t;
    if(fabs(a)>fabs(b)){
        t = a/b;
        G->s = ((b > 0) - (b < 0))/sqrt(1+pow(t,2));
        G->c = G->s * t;
    }
    else{
        t = b/a;
        G->c = ((a > 0) - (a < 0))/sqrt(1+pow(t,2));
        G->s = G->c * t;
    }
}



double* matmul(double* A, double* B, int l, int m, int n){
    double* C = calloc(l*n, sizeof(double));
    for(int i=0; i<l; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<m;k++){
                C[i*n+j] += A[k*m+i] * B[k*n+j];
            }
        }
    }
    return C;
}

void printm(double* A, int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n;j++){
            printf("%f ", A[i*n + j]);
        }
        printf("\n");
    }
}

void check_factorization(double* A, double* Q, double* R, int m, int n){
    double* A_prime = matmul(Q, R, m, m, n);
    double diff=0;
    for(int i=0; i<m*n; i++){
        diff += fabs(A[i] - A_prime[i]);
    }
    if (diff > 0.1){
        printf("WARNING: Check failed, the difference is %f\n", diff);
    }
    free(A_prime);
}

double* transpose(double* A, int n){
    double* A_out = malloc(n*n*sizeof(double));
    for(int i=0; i<n; i++){
        for(int j=0; j<n;j++){
            A_out[j*n+i] = A[i*n+j];
        }
    }
    free(A);
    return A_out;
}

void factorize(double* A, double** Q_p, double** R_p, int m, int n){
    double* Q = calloc(m*m, sizeof(double));
    double* R = malloc(m*n*sizeof(double));
    // init Q and R
    for(int i=0; i<m; i++){
        Q[i*m+i] = 1;
    }
    for(int i=0; i<m*n; i++){
        R[i] = A[i];
    }
    givens_t g;

    for(int j=0; j<n; j++){
        for(int i=m-1; i>j; i--){
            compute_givens_factors(R[(i-1)*n+j], R[i*n+j], &g);
            g.i = i;
            g.j = i-1;
            matmul_givens_R(g, R, m, n);
            matmul_givens_Q(g, Q, m);
        }
    }
    *Q_p = Q;
    *R_p = R;
}   

double* create_random(int i, int j){
    double* M = malloc(i*j*sizeof(double));
    double max= 1;
    double min=-1;
    double range = (max - min); 
    double div = RAND_MAX / range;
    for(int id=0; id<i*j; id++){
        M[id] = min + (rand() / div);
    }
    return M;
}

int main(int argc, char *argv[]){

#if !USE_EXAMPLE
    if(argc != 3) {
    printf("Give 2 input args: m, n\n");
    return -1;
    }
    const int i = atoi(argv[1]);
    const int j = atoi(argv[2]);
    double* T = create_random(i,j);
#else
    int i = 5;
    int j = 3;
    double T[] = {
    0.8147, 0.0975, 0.1576,
    0.9058, 0.2785, 0.9706,
    0.1270, 0.5469, 0.9572,
    0.9134, 0.9575, 0.4854,
    0.6324, 0.9649, 0.8003
    };
#endif


    double* Q, *R;
    double start = get_wall_seconds();
    factorize(T, &Q, &R, i, j);
    double end = get_wall_seconds();
    printf("The execution took %f seconds\n", end-start);
    // printf("Checking matrices\n");
    check_factorization(T, Q, R, i, j);
    free(Q);
    free(R);
#if !USE_EXAMPLE    
    free(T);
#endif
    return 0;
}