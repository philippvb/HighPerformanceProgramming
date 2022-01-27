#include <stdio.h>
#include <stdlib.h>

#define ROWS 5
#define COLS 5

int main(){
    int array[ROWS][COLS];
    int cur_value;
    // fill the array
    for(int row=0; row < ROWS; row++){
        for(int col=0; col<row; col++){
            array[row][col] = -1;
        }
        array[row][row] = 0;
        for(int col=row+1; col<COLS; col++){
            array[row][col] = 1;
        }
    }

    // display it
    for(int row=0; row < ROWS; row++){
        for(int col=0; col<COLS; col++){
            printf("%d\t" , array[row][col]);
        }
        printf("\n");
    }

}