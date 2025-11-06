#include "sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <array_size> <num_threads> <mode>\n", argv[0]);
    fprintf(stderr, "<mode>: 1 = ascending, 0 = descending\n");
    return 1;
  }

  int n = atoi(argv[1]);
  int p = atoi(argv[2]);
  int ascending = atoi(argv[3]);

  if (n <= 0 || p <= 0 || (ascending != 0 && ascending != 1)) {
    fprintf(stderr, "Invalid arguments.\n");
    return 1;
  }

  int *arr = malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("Pthreads Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);
  printf("Mode: %s\n", ascending ? "Ascending" : "Descending");

  double start_time = get_time();
  insertionSort_pthread(arr, n, p, ascending);
  double end_time = get_time();

  printf("Execution time: %f seconds\n", end_time - start_time);

  free(arr);
  return 0;
}
