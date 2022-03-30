#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define EXAMPLE 1


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
        col_copy[i] = X[i * n + g.i];
        col_copy[n+i] = X[i * n + g.j];
    }
    // iterate over all cols
    for(int k=0; k<n; k++){
        // X_ki = c * A_ki - s * A_kj
        X[k*n + g.i] = g.c * col_copy[k] - g.s * col_copy[n+k];

        // X_kj = s * A_ik + c * A_kj
        X[k*n + g.j] = g.s * col_copy[k] + g.c * col_copy[n+k];        
    }
    free(col_copy);
}

// /**
//  * @brief computes the naive givens factors for a given A and B
//  * 
//  * @param a the a in the col
//  * @param b the b in the col
//  */
// double* compute_givens_factors(double a, double b){
//     // printf("%f, %f\n", a, b);
//     double* sol = malloc(2*sizeof(double));
//     double r = 1/sqrt(a*a + b*b);
//     sol[0] = a*r;
//     sol[1] = b*r;
//     return sol;
// }

inline void compute_givens_factors(double a, double b, givens_t* G){
    double r = 1/sqrt(a*a + b*b);
    G->c = a*r;
    G->s = b*r;
}



double* matmul(double* A, double* B, int l, int m, int n){
    double* C = calloc(l*n, sizeof(double));
    for(int i=0; i<l; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<m;k++){
                C[i*n+j] += A[i*m+k] * B[k*n+j];
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
    // printf("Checking factorization\n");
    // printm(A_prime, m, n);
    // printf("\n");
    // printm(A, m, n);
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
    double* G_int_factors;
    givens_t g;
    // printm(Q, m, m);
    // printm(R, m, n);

    // printf("Starting factorization\n");

    for(int j=0; j<n; j++){
        for(int i=m-1; i>j; i--){
            compute_givens_factors(R[(i-1)*n+j], R[i*n+j], &g);
            // printf("i %d, j %d\n", i, j);
            // printf("%f, %f\n", R[(i-1)*n+j], R[i*n+j]);
            // printf("c:%f, s: %f \n", G_int_factors[0], G_int_factors[1]);
            g.i = i;
            g.j = i-1;
            matmul_givens_R(g, R, m, n);
            // printm(R, m, n);
            matmul_givens_Q(g, Q, m);
        // printm(Q, m, m);
        }
    }
    // printf("Finished\n");
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

#if EXAMPLE
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
#if EXAMPLE    
    free(T);
#endif
    // G_int_factors = compute_givens_factors(T[(4-1)*j+1], T[4*j+1]);
    // // printf("c:%f, s: %f \n", G_int_factors[0], G_int_factors[1]);
    // G_int = create_givens(4, 4-1, G_int_factors[0], G_int_factors[1], i);
    // // printm(G_int, m, m);
    // R_new = matmul(G_int, T, i, i, j);
    // printm(R_new, i, j);
    // printf("\n");

    // givens_t g = {4, 4-1, G_int_factors[0], G_int_factors[1]};
    // matmul_givens_R(g, T, i, j);
    // printm(T, i, j);
    return 0;
}