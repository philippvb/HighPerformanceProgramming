#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct givens
{
    int i;
    int j;
    double c;
    double s;
} givens_t;

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
    // printf("%f, %f\n", a, b);
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


// void matmul(double* A, double* B, double* C, int l, int m, int n){

//     for(int i=0; i<l; i++){
//         for(int j=0; j<n; j++){
//             for(int k=0; k<m;k++){
//                 C[i*l+j] += A[i*l+k] * B[k*m+j];
//             }
//         }
//     }
// }

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
    printf("Checking factorization");
    printm(A, m, n);
    printf("\n");
    printm(A_prime, m, n);
    double diff=0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            diff += fabs(A[i*n+j] - A_prime[i*n+j]);
        }
    }
    printf("The difference is %f", diff);
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
    printf("\n");
    printm(Q, 5,5);
    printf("\n");
    printm(R, m, n);
    double* G_int_factors, *G_int;
    double* Q_new, *R_new;

    for(int j=0; j<n; j++){
        for(int i=m-1; i>j; i--){
            printf("i: %d, j: %d, a: %f, b: %f\n", i, j, R[(i-1)*n+j], R[i*n+j]);
            G_int_factors = compute_givens_factors(R[(i-1)*n+j], R[i*n+j]);
            // printf("c:%f, s: %f \n", G_int_factors[0], G_int_factors[1]);
            G_int = create_givens(i, i-1, G_int_factors[0], G_int_factors[1], m);
            printm(G_int, 5, 5);
            R_new = matmul(G_int, R, m, m, n);
            free(R);
            R = R_new;
            printm(R_new, m, n);
            G_int = transpose(G_int, 5);
            Q_new = matmul(Q, G_int, m, m, m);
            free(Q);
            Q = Q_new;
        }
    }
    printf("Finished\n");
    printm(R, m, n);
    printm(Q, m, m);
    *Q_p = Q;
    *R_p = R;
}   


int main(){
    printf("Start\n"); 
    // double x[4] = {1.0, 2.0, 3.0, 4.0};
    // double id[4] = {1.0, 0.0, 0.0, 1.0};
    // double C[3*3];
    // for(int i=0; i<4;i++)
    //     C[i] = 0;
    // printm(x, 2, 2);
    // printf("\n");
    // matmul(x,x, C, 2, 2, 2);
    // printm(C, 2, 2);
    // check_factorization(x, x, id, 2);

    // double* C = calloc(4*4, sizeof(double));
    // double* A = create_givens(1, 3, 1.5, 0.5, 4);
    // double* B = create_givens(2, 3, 2.5, 3.5, 4);
    // matmul(A, B, C, 4,4,4);
    // printm(A, 4, 4);
    // printf("\n");
    // printm(B, 4, 4);
    // printf("\n");
    // printm(C, 4, 4);
    // free(A);
    // free(B);
    // free(C);
    // double* sol = compute_givens_factors(0.6585, -0.4636);
    // printf("%f, %f\n", sol[0], sol[1]);
    double T[] = {
    0.8147, 0.0975, 0.1576,
    0.9058, 0.2785, 0.9706,
    0.1270, 0.5469, 0.9572,
    0.9134, 0.9575, 0.4854,
    0.6324, 0.9649, 0.8003
    };
    printm(T, 5, 3);

    // double* id = calloc(3*3, sizeof(double));
    // for(int i=0; i<3; i++)
    //     id[i*3+i]=1;
    // // printm(id, 5, 5);
    // printf("\n");
    // printm(matmul(T, id, 5, 3, 3), 5, 3);
    double* Q, *R;
    factorize(T, &Q, &R, 5, 3);
    check_factorization(T, Q, R, 5, 3);
    

}