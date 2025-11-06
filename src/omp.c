#include "sort.h"
#include <omp.h>
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

  int *arr = (int *)malloc(n * sizeof(int));
  generate_array(arr, n);

  // ---- SEQ BASELINE ----
  int *arr_seq = (int *)malloc(n * sizeof(int));
  copy_array(arr, arr_seq, n);

  double seq_start = get_time();
  insertionSort(arr_seq, n, 1);
  double seq_end = get_time();
  double seq_time = seq_end - seq_start;

  printf("OpenMP Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Sequential time: %f s\n\n", seq_time);

  free(arr_seq);

  // ---- PARALLEL TEST ----
  int p_values[] = {2, 4, 6, 8, 10, 12};
  int p_count = sizeof(p_values) / sizeof(p_values[0]);

  printf("Threads\tTime(s)\tSpeedup\tEfficiency\n");
  printf("--------------------------------------------\n");

  for (int i = 0; i < p_count; i++) {
    int p = p_values[i];
    omp_set_num_threads(p);

    int *arr_copy = (int *)malloc(n * sizeof(int));
    copy_array(arr, arr_copy, n);

    double t0 = get_time();
    insertionSort_omp(arr_copy, n, 1);
    double t1 = get_time();

    double par_time = t1 - t0;
    double speedup = seq_time / par_time;
    double efficiency = speedup / p;

    printf("%d\t%f\t%f\t%f\n", p, par_time, speedup, efficiency);

    free(arr_copy);
  }

  free(arr);
  return 0;
}
