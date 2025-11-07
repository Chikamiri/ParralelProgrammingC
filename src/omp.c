#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <array_size> <num_threads>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  int p = atoi(argv[2]);

  if (n <= 0 || p <= 0) {
    fprintf(stderr,
            "Array size and number of threads must be positive integers.\n");
    return 1;
  }

  omp_set_num_threads(p);

  int *arr = malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("OpenMP Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);

  double start_time = get_time();
  insertionSort_omp(arr, n, 1); // 1 = sort ascending
  double end_time = get_time();

  printf("Execution time: %f seconds\n", end_time - start_time);

  free(arr);
  return 0;
}
