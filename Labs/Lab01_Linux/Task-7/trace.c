#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double** initialization(int);
void fill_vectors(double*, int);
void print_matrix(double** , int);
double trace (double** , int); //no pointer
void cleanup(double** , int);

// The main program.
int main()
{
  int i, n; // j was not used
  double **matrix;
  double sum =0;
  printf("\nEnter the Dimension for a square matrix:");
  scanf("%d",&n); // we need a pointer to n for scanf
  matrix = initialization(n);
  for(i = 0 ; i < n ; i++) //start at 0
  fill_vectors(matrix[i] , n);
  sum = trace(matrix , n); //no pointer
  print_matrix(matrix , n);
  printf("\n Sum of the diagonal elements are: %2.3f", sum); // no pointer
  cleanup(matrix, n);
  return 0;
}

void cleanup(double **matrix, int n){ // add cleanup function
  for(int row=0; row<n; row++){
    free(matrix[row]);
  }
  free(matrix);
}


// The initialization routine is supposed to allocate memory for
// a n-by-n matrix and return a pointer to the allocated matrix.
double** initialization(int n)

{
  int i;
  double** matrix ;
  matrix = calloc(n , sizeof(double *));
  for(i=0 ; i< n ; ++i)
    matrix[i] = calloc(n , sizeof(double));
  return matrix; //no brackets
}

// The fill_vector routine is supposed to fill a given vector with
// random numbers ranging from -10 to 10.
void fill_vectors(double* vec , int n) //function is called fill vectors
{
  int i ;
  for(i = 0 ; i < n ; i++)
    vec[i] = rand() % 20 - 10 ;
}

// The print_matrix routine is supposed to print out the elements of
// a given matrix in a nice format using printf.
void print_matrix(double** matrix , int n)

{
  int i,j;
  for (i= 0 ; i< n ; i++)
    {
    for(j = 0 ; j < n ; j++)
      printf("%2.3f " , matrix[i][j]); // need to print float instead of int
    putchar('\n');
    }
}

// The trace routine is supposed to return the sum of the diagonal
// elements if a given matrix.
double trace (double** matrix , int n)
{
  int i ;
  double sum = 0.0;
  for(i=0 ; i<n ; i++)
    sum+=matrix[i][i];
  return sum; // we dont want to return the pointer
}
