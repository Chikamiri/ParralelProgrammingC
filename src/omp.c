#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void insertionSort_omp(int arr[], int n) {
  // Hint: A simple approach is to parallelize the outer loop with '#pragma omp
  // parallel for'. However, insertion sort has dependencies that make this
  // inefficient. A better strategy is a parallel merge sort: divide the array,
  // sort chunks in parallel, then merge. This function is a placeholder. For a
  // real implementation, consider a different algorithm.

  // Placeholder: calling sequential sort
  insertionSortAscending(arr, n);
}

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

  int *arr = (int *)malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("OpenMP Insertion Sort (Placeholder)\n");
  printf("Array size: %d\n", n);

  // Loop through specified thread counts for benchmarking
  int p_values[] = {2, 4, 6, 8, 10, 12};
  int num_p_values = sizeof(p_values) / sizeof(p_values[0]);

  for (int i = 0; i < num_p_values; i++) {
    int p = p_values[i];
    omp_set_num_threads(p);

    int *arr_copy = (int *)malloc(n * sizeof(int));
    copy_array(arr, arr_copy, n);

    double start_time = get_time();
    insertionSort_omp(arr_copy, n);
    double end_time = get_time();

    printf("Threads: %d, Execution time: %f seconds\n", p,
           end_time - start_time);
    free(arr_copy);
  }

  free(arr);
  return 0;
}
