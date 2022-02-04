#include <stdlib.h>
#include "sort_funcs.h"

#define MIN_SIZE 10

void bubble_sort(int* list, int N) {
  int i, j;
  for(i = 0; i < N-1; i++)
    for(j = 1+i; j < N; j++) {
      if(list[i] > list[j]) {
	// Swap
	int tmp = list[i];
	list[i] = list[j];
	list[j] = tmp;
      }
    }
}

void merge_sort(int* list_to_sort, int N) {
  if(N <= MIN_SIZE) {
    // Go to bubblesort in this case
    bubble_sort(list_to_sort, N);
    return;
  }
  int n1 = N / 2;
  int n2 = N - n1;
  // Allocate new lists
  int* list1;
  int* list2;
  int buffer[N];// = (int*) malloc(N*sizeof(int));
  list1 = buffer;
  list2 = &buffer[n1];
  int i;
  for(i = 0; i < n1; i++)
    list1[i] = list_to_sort[i];
  for(i = 0; i < n2; i++)
    list2[i] = list_to_sort[n1+i];
  // Sort list1 and list2
  merge_sort(list1, n1);
  merge_sort(list2, n2);
  // Merge!
  int i1 = 0;
  int i2 = 0;
  i = 0;
  while(i1 < n1 && i2 < n2) {
    if(list1[i1] < list2[i2]) {
      list_to_sort[i] = list1[i1];
      i1++;
    }
    else {
      list_to_sort[i] = list2[i2];
      i2++;
    }
    i++;
  }
  while(i1 < n1)
    list_to_sort[i++] = list1[i1++];
  while(i2 < n2)
    list_to_sort[i++] = list2[i2++];
  // free(buffer);
}

