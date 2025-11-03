#include "sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Hint: Define a struct to pass data (array chunk, size) to each thread.
/*
typedef struct {
    int *arr;
    int size;
} thread_arg_t;
*/

// Hint: This would be the worker function for each thread.
/*
void *sort_worker(void *arg) {
    thread_arg_t *data = (thread_arg_t *)arg;
    insertionSortAscending(data->arr, data->size);
    pthread_exit(NULL);
}
*/

void insertionSort_pthread(int arr[], int n, int num_threads) {
  // Hint: In main, create 'p' threads. Each thread sorts its chunk.
  // After joining all threads, perform a k-way merge on the sorted chunks.
  // This function is a placeholder.

  // Placeholder: calling sequential sort
  insertionSortAscending(arr, n);
}

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

  int *arr = (int *)malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("Pthreads Insertion Sort (Placeholder)\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);

  double start_time = get_time();
  insertionSort_pthread(arr, n, p);
  double end_time = get_time();

  printf("Execution time: %f seconds\n", end_time - start_time);

  free(arr);
  return 0;
}
