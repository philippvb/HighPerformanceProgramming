#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 3


int main()
{

int *memory_arr = (int *)malloc(STACK_SIZE*sizeof(int));
memory_arr[0] = 123;
memory_arr = (int *) realloc(memory_arr, 5*sizeof(int));
for(int i=0; i<5; i++){
        printf("%d", memory_arr[i]);
}
// printf("%d", next_value);

return 0; }