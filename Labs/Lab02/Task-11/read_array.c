#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 10

void extend_and_transer_stack(int stack_array[], int *memory_array[], int n_stack_elements, int *n_mem_elements, int *memory_size){
    // extend memory if needed
    if((n_stack_elements + *n_mem_elements) > *memory_size){
        *memory_array = realloc(*memory_array, (n_stack_elements + *n_mem_elements)*sizeof(int));
        *memory_size = n_stack_elements + *n_mem_elements;
    }
    // copy array
    memcpy(*memory_array + *n_mem_elements, stack_array, n_stack_elements*sizeof(int));   
    *n_mem_elements += n_stack_elements;
}

int main()
{

int stack_arr[STACK_SIZE];
int stack_count=0;
int *memory_arr = (int *)malloc(0);
int memory_size = 0;
int memory_count = 0;

int total_count = 0;
printf("Enter array values: ");
while(scanf("%d", stack_arr + stack_count)){
    total_count ++;
    stack_count++;
    if(stack_count == STACK_SIZE){
        extend_and_transer_stack(stack_arr, &memory_arr, stack_count, &memory_count, &memory_size);
        stack_count = 0;
    }
}

// finally flush everthing from stackarray
extend_and_transer_stack(stack_arr, &memory_arr, stack_count, &memory_count, &memory_size);


int memory_sum=0;
printf("Output: ");
for(int i=0; i<memory_size; i++){
        printf("%d ", memory_arr[i]);
        memory_sum += memory_arr[i];
}
printf("\nSum: %d", memory_sum);
free(memory_arr);

return 0; }