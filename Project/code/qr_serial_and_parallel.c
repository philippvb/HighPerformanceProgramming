#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <unistd.h>

// Wether to run a predefined example https://www.math.usm.edu/lambers/mat610/class0208.pdf, mostly used for checking
#define RUN_EXAMPLE 0

// The values on the lower diagonal will not exactly be 0 due to rounding
// errors, this is the tolerance for checking
#define EPS 1e-15
int n_threads;

// utils functions
void printm(double* A, int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n;j++){
            printf("%.10f ", A[i*n + j]);
        }
        printf("\n");
    }
}

/**
 * @brief Create a random 2D-matrix with values between +-1
 * 
 * @param i dim 0
 * @param j dim 1
 * @return double* The matrix, in row format
 */
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


inline int min(int a, int b){
    return (a<b) * a + (b<=a) * b;
}

inline int max(int a, int b){
    return (a>b) * a + (b>=a) * b;
}

inline int triangular_number(int n){
    return (n * n + n)/2;
}

/**
 * @brief Get wall seconds, used for timing
 * 
 * @return double 
 */
double get_wall_seconds(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
  return seconds;
}



/**
 * @brief Representation of a Givens rotation matrix
 * 
 */
typedef struct givens{
// indices for non-zero objects
  int i;
  int j;
// values stored at non-zero indices
  double c;
  double s;
} givens_t;

// matrix multiplications

/**
 * @brief computes the matmul GR, where G is Givens matrix, G mxm
 * 
 * @param g The Givens matrix G
 * @param X The factorization matrix R, in row format
 * @param n The dimension m
 */
inline void matmul_givens_R(givens_t g, double* R, int m, int n){
    // copy the two rows in intermediate matrix
    double* row_copy = malloc(2*n*sizeof(double));
    for(int i=0; i<n; i++){
        row_copy[i] = R[g.i*n + i];
        row_copy[n+i] = R[g.j*n + i];
    }
    // iterate over all columns, compute the matmul
    for(int k=0; k<n; k++){
        // R_ik = c * R_ik - s * R_jk
        R[g.i*n + k] = g.c * row_copy[k] - g.s * row_copy[n+k];

        // R_jk = s * R_ik + c * R_jk
        R[g.j*n + k ] = g.s * row_copy[k] + g.c * row_copy[n+k];        
    }
    free(row_copy);
}

/**
 * @brief computes the matmul QG, where G is Givens matrix, G mxm
 * 
 * @param g The Givens matrix G
 * @param Q The factorization matrix Q, in column format
 * @param n The dimension m
 */
inline void matmul_givens_Q(givens_t g, double* Q, int m){
    // copy the two cols in intermediate matrix
    double* col_copy = malloc(2*m*sizeof(double));
    for(int i=0; i<m; i++){
        col_copy[i] = Q[g.i * m + i];
        col_copy[m+i] = Q[g.j * m + i];
    }
    // iterate over all rows, compute the matmul
    for(int k=0; k<m; k++){
        // Q_ki = c * Q_ki - s * Q_kj
        Q[g.i * m + k] = g.c * col_copy[k] - g.s * col_copy[m+k];
        // Q_kj = s * Q_ik + c * Q_kj
        Q[g.j * m + k] = g.s * col_copy[k] + g.c * col_copy[m+k];        
    }
    free(col_copy);
}

/**
 * @brief Naive matrix multiplication C=AB, just used for testing, therefore not optimized
 * 
 * @param A matrix A, dim: lxm, column format
 * @param B matrix A, dim: mxn, row format
 * @param l dimension 0
 * @param m dimension 1
 * @param n dimension 2
 * @return double*  The result matrix C
 */
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


/**
 * @brief computes the stable Givens factors for a given A and B and saves it in G
 * 
 * @param a param "a" of Givens rotation
 * @param b param "b" of Givens rotation
 * @param G pointer to the givens rotation object to write to
 */

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


/**
 * @brief Checks the QR factorization A=QR for correctness, A mxn
 * 
 * @param A see all in description
 * @param Q 
 * @param R 
 * @param m 
 * @param n 
 */
void check_factorization(double* A, double* Q, double* R, int m, int n){

    double* A_prime = matmul(Q, R, m, m, n);
    // first we check if Q is upper triangular
    for(int i=0; i<m; i++){
        for(int j=0; j<min(i, n); j++){
            if(fabs(R[i*n+j]) >= EPS){
                printf("WARNING: Check failed, the lower triangle of R seems to contain other values than 0 at pos %d, %d, value %f\n", i, j, R[i*n+j]);
                return;
            }
            // set values to 0, since during factorization rounding error occures
            R[i*n+j] = 0.0;
        }
    }
    // check the actual factorization
    double diff=0;
    for(int i=0; i<m*n; i++){
        diff += fabs(A[i] - A_prime[i]);
    }
    if (diff > 0.1){
        printf("WARNING: Check failed, the difference is %f\n", diff);
    }
    free(A_prime);
}

/**
 * @brief Computes the QR factorization A=QR, A mxn
 * 
 * @param A The input matrix
 * @param Q_p Pointer variable to Q
 * @param R_p Pointer variable to R
 * @param m dim 0 of A
 * @param n dim 1 of A
 */
void factorize(double* A, double** Q_p, double** R_p, int m, int n){
    double* Q = calloc(m*m, sizeof(double));
    double* R = malloc(m*n*sizeof(double));
    givens_t g;
    // init Q and R
    for(int i=0; i<m; i++){
        Q[i*m+i] = 1;
    }
    for(int i=0; i<m*n; i++){
        R[i] = A[i];
    }

    // Iterate, stepwise eliminate values
    for(int it = 0; it<(n-1)*2+m-n; it++){
        // parallel block
        #pragma omp parallel for num_threads(n_threads) private(g)
        for(int j=max(0, it-m+2); j<=min(it/2, n-1); j+=1){
            int i = (m-1-it+2*j);
            // compute the givens factors
            compute_givens_factors(R[(i-1)*n+j], R[i*n+j], &g);
            g.i = i;
            g.j = i-1;
            // update Q and R
            matmul_givens_R(g, R, m, n);
            matmul_givens_Q(g, Q, m);
        }
    }
    // set pointers
    *Q_p = Q;
    *R_p = R;
}   


int main(int argc, char *argv[]){

#if !RUN_EXAMPLE
    if(argc != 4) {
    printf("Give 3 input args: m, n, n_threads\n");
    return -1;
    }
    const int i = atoi(argv[1]);
    const int j = atoi(argv[2]);
    double* T = create_random(i,j);
    n_threads = atoi(argv[3]);
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
    n_threads = 1;
#endif
    double* Q, *R;
    double start = get_wall_seconds();
    factorize(T, &Q, &R, i, j);
    double end = get_wall_seconds();
    printf("The execution took %f seconds\n", end-start);
    check_factorization(T, Q, R, i, j);
    free(Q);
    free(R);
#if !RUN_EXAMPLE    
    free(T);
#endif
    return 0;
}