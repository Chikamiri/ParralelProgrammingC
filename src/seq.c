#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <array_size>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  if (n <= 0) {
    fprintf(stderr, "Array size must be a positive integer.\n");
    return 1;
  }

  // Allocate memory for arrays
  int *arr = (int *)malloc(n * sizeof(int));
  int *arr_copy = (int *)malloc(n * sizeof(int));
  if (arr == NULL || arr_copy == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return 1;
  }

  // Generate a random array
  generate_array(arr, n);
  copy_array(arr, arr_copy, n); // Keep a copy for the second sort

  printf("Sequential Insertion Sort\n");
  printf("Array size: %d\n", n);

  // Time and execute the ascending sort
  double start_time = get_time();
  insertionSortAscending(arr, n);
  double end_time = get_time();
  printf("Ascending sort time: %f seconds\n", end_time - start_time);

  // Time and execute the descending sort
  start_time = get_time();
  insertionSortDescending(arr_copy, n);
  end_time = get_time();
  printf("Descending sort time: %f seconds\n", end_time - start_time);

  // Clean up
  free(arr);
  free(arr_copy);

  return 0;
}
