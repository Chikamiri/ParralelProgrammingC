#include "sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr,
            "Usage: %s <array_size> <num_threads> <mode> <merge_version>\n",
            argv[0]);
    fprintf(stderr, "<mode>: 1 = ascending, 0 = descending\n");
    fprintf(stderr, "<merge_version>: 0=serial, 1=parallel, 2=tree\n");
    return 1;
  }

  int n = atoi(argv[1]);
  int p = atoi(argv[2]);
  int ascending = atoi(argv[3]);
  int merge_v = atoi(argv[4]);

  if (n <= 0 || p <= 0 || (ascending != 0 && ascending != 1) || merge_v < 0 ||
      merge_v > 2) {
    fprintf(stderr, "Invalid arguments.\n");
    return 1;
  }

  int *arr = malloc(n * sizeof(int));
  generate_array(arr, n);

  MergeVersion merge_ver = (MergeVersion)merge_v;

  printf("Pthreads Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);
  printf("Mode: %s\n", ascending ? "Ascending" : "Descending");
  printf("Merge version: %s\n", merge_ver == 0   ? "Serial"
                                : merge_ver == 1 ? "Parallel"
                                                 : "Tree");

  double start_time = get_time();
  insertionSort_pthread(arr, n, p, ascending, merge_ver);
  double end_time = get_time();

  printf("Total execution time (sort+merge): %f seconds\n",
         end_time - start_time);

  free(arr);
  return 0;
}
