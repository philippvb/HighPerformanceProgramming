#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void matmul(double* A, double* B, double* C, int l, int m, int n){

    for(int i=0; i<l; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<m;k++){
                C[i*l+j] += A[i*l+k] * B[k*m+j];
            }
        }
    }
}

void printm(double* A, int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n;j++){
            printf("%f ", A[i*m + j]);
        }
        printf("\n");
    }
}

void check_factorization(double* A, double* Q, double* R, int n){
    double* A_prime = calloc(n*n, sizeof(double));
    matmul(Q, R, A_prime, n, n, n);
    double diff=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            diff += fabs(A[i*n+j] - A_prime[i*n+j]);
        }
    }
    printf("The difference is %f", diff);
    free(A_prime);
}

int main(){
    printf("Start\n"); 
    double x[4] = {1.0, 2.0, 3.0, 4.0};
    double id[4] = {1.0, 0.0, 0.0, 1.0};
    double C[2*2];
    for(int i=0; i<4;i++)
        C[i] = 0;
    printm(x, 2, 2);
    printf("\n");
    matmul(x,x, C, 2, 2, 2);
    printm(C, 2, 2);
    check_factorization(x, x, id, 2);
}