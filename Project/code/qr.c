#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief computes the naive givens factors for a given A and B
 * 
 * @param a the a in the col
 * @param b the b in the col
 * @return double* the givens factors in the form [c,s]
 */
double* compute_givens_factors(double a, double b){
    // printf("%f, %f\n", a, b);
    double* sol = malloc(2*sizeof(double));
    double r = sqrt(pow(a, 2) + pow(b, 2));
    sol[0] = a/r;
    sol[1] = b/r;
    // if(sol[0] < 0){
    //     sol[0] *=-1;
    //     sol[1] *=-1;
    // }
    return sol;
}

double* compute_givens_factors_2(double a, double b){
    double* sol = malloc(2*sizeof(double));
    double t;
    if(fabs(a)>fabs(b)){
        t = a/b;
        sol[1] = ((b > 0) - (b < 0))/sqrt(1+pow(t,2));
        sol[0] = sol[1] * t;
    }
    else{
        t = b/a;
        sol[0] = ((a > 0) - (a < 0))/sqrt(1+pow(t,2));
        sol[1] = sol[0] * t;
    }
    return sol;
}




/**
 * @brief Create a naive full givens matrix
 * 
 * @param i col
 * @param j row
 * @param c value for diagonal
 * @param s value for off diagonal
 * @param n size of matrix
 * @return double* the matrix
 */
double* create_givens(int i, int j, double c, double s, int n){
    double* G = calloc(n*n, sizeof(double));
    for(int i=0; i<n;i++){
        G[i*n+i] = 1;
    }
    G[i*n+i] = c;
    G[j*n+j] = c;
    G[i*n+j] = -s;
    G[j*n+i] = s;
    return G;
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
    printf("Checking factorization\n");
    double diff=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            diff += fabs(A[i*n+j] - A_prime[i*n+j]);
        }
    }
    printf("The difference is %f\n", diff);
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
    double* G_int_factors, *G_int;
    double* Q_new, *R_new;

    printf("Starting factorization\n");

    for(int j=0; j<n; j++){
        for(int i=m-1; i>j; i--){
            G_int_factors = compute_givens_factors(R[(i-1)*n+j], R[i*n+j]);
            G_int = create_givens(i, i-1, G_int_factors[0], G_int_factors[1], m);
            R_new = matmul(G_int, R, m, m, n);
            free(R);
            R = R_new;
            G_int = transpose(G_int, m);
            Q_new = matmul(Q, G_int, m, m, m);
            free(Q);
            Q = Q_new;
        }
    }
    printf("Finished\n");
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

int main(){ 
    int i = 20;
    int j= 10;
    double* T = create_random(i,j);
    double* Q, *R;
    factorize(T, &Q, &R, i, j);
    check_factorization(T, Q, R, i, j);
    return 0;
}