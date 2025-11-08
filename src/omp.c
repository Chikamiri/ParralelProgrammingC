#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <array_size> <num_threads> <merge_version>\n",
            argv[0]);
    fprintf(stderr, "<merge_version>: 0=serial, 1=parallel, 2=tree\n");
    return 1;
  }

  int n = atoi(argv[1]);
  int p = atoi(argv[2]);
  int merge_v = atoi(argv[3]);

  if (n <= 0 || p <= 0 || merge_v < 0 || merge_v > 2) {
    fprintf(stderr, "Invalid arguments.\n");
    return 1;
  }

  omp_set_num_threads(p);

  int *arr = malloc(n * sizeof(int));
  generate_array(arr, n);

  MergeVersion merge_ver = (MergeVersion)merge_v;

  printf("OpenMP Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);
  printf("Merge version: %s\n", merge_ver == 0   ? "Serial"
                                : merge_ver == 1 ? "Parallel"
                                                 : "Tree");

  double start_time = get_time();
  insertionSort_omp(arr, n, 1, merge_ver); // 1 = ascending
  double end_time = get_time();

  printf("Total execution time (sort+merge): %f seconds\n",
         end_time - start_time);

  free(arr);
  return 0;
}
