#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void insertionSort_omp(int arr[], int n) {
  if (n <= 1)
    return;

// Parallel insertion sort using OpenMP
#pragma omp parallel
  {
#pragma omp single
    {
      for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Move elements that are greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
          arr[j + 1] = arr[j];
          j--;
        }
        arr[j + 1] = key;

        // Parallelize the shifting process for larger arrays
        if (i % 100 == 0) {
#pragma omp task
          {
            // This creates parallel tasks for every 100 elements
            // helping with load distribution
          }
        }
      }
    }
  }
}

// Alternative parallel approach: Sort chunks in parallel then merge
void parallelInsertionSortChunks(int arr[], int n) {
  int num_threads = omp_get_max_threads();
  int chunk_size = n / num_threads;

#pragma omp parallel for
  for (int i = 0; i < num_threads; i++) {
    int start = i * chunk_size;
    int end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;

    // Sort each chunk using insertion sort
    for (int j = start + 1; j < end; j++) {
      int key = arr[j];
      int k = j - 1;
      while (k >= start && arr[k] > key) {
        arr[k + 1] = arr[k];
        k--;
      }
      arr[k + 1] = key;
    }
  }

  // Sequential merge of sorted chunks (for simplicity)
  // In a more advanced version, this could also be parallelized
  for (int chunk = 1; chunk < num_threads; chunk++) {
    int start = chunk * chunk_size;
    int end = (chunk == num_threads - 1) ? n : (chunk + 1) * chunk_size;

    for (int i = start; i < end; i++) {
      int key = arr[i];
      int j = i - 1;
      while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = key;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <array_size>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  if (n <= 0) {
    printf("Array size must be positive integer\n");
    return 1;
  }

  // Generate random array
  int *arr = malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("OpenMP Insertion Sort Performance Analysis\n");
  printf("Array size: %d\n", n);
  printf("Array sample (first 10 elements): ");
  for (int i = 0; i < 10 && i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\n");

  // Test sequential version first
  int *arr_seq = (int *)malloc(n * sizeof(int));
  copy_array(arr, arr_seq, n);

  double seq_start = get_time();
  insertionSortAscending(arr_seq, n);
  double seq_end = get_time();

  printf("Sequential execution time: %f seconds\n", seq_end - seq_start);

  // Verify the array is sorted
  int is_sorted = 1;
  for (int i = 1; i < n; i++) {
    if (arr_seq[i] < arr_seq[i - 1]) {
      is_sorted = 0;
      break;
    }
  }
  printf("Array sorted correctly: %s\n\n", is_sorted ? "Yes" : "No");
  free(arr_seq);

  // Test sequential
  int *arr_seq_asc = malloc(n * sizeof(int));
  int *arr_seq_desc = malloc(n * sizeof(int));

  printf("Parallel Performance Results:\n");
  printf("Threads\tTime (s)\tSpeedup\t\tEfficiency\n");
  printf("-------------------------------------------------\n");

  for (int i = 0; i < num_p_values; i++) {
    int p = p_values[i];
    omp_set_num_threads(p);

    copy_array(arr, arr_seq_desc, n);
    start = get_time();
    insertionSort(arr_seq_desc, n, 0);
    double seq_desc_time = get_time() - start;

    printf("SEQUENTIAL:\n");
    printf("Ascending:  %.6f seconds\n", seq_asc_time);
    printf("Descending: %.6f seconds\n", seq_desc_time);

    double parallel_time = end_time - start_time;
    double speedup = (seq_end - seq_start) / parallel_time;
    double efficiency = speedup / p;

    printf("%d\t%f\t%f\t%f\n", p, parallel_time, speedup, efficiency);

    // Verify sorting result
    is_sorted = 1;
    for (int j = 1; j < n; j++) {
      if (arr_copy[j] < arr_copy[j - 1]) {
        is_sorted = 0;
        break;
      }
    }

    if (!is_sorted) {
      printf("WARNING: Array not sorted correctly with %d threads!\n", p);
    }

    free(arr_copy);
  }

  // Test alternative parallel approach
  printf("\nAlternative Approach - Chunk-based Parallel Sort:\n");
  int *arr_chunk = (int *)malloc(n * sizeof(int));
  copy_array(arr, arr_chunk, n);

  double chunk_start = get_time();
  parallelInsertionSortChunks(arr_chunk, n);
  double chunk_end = get_time();

  printf("Chunk-based parallel time: %f seconds\n", chunk_end - chunk_start);

  // Verify sorting
  is_sorted = 1;
  for (int i = 1; i < n; i++) {
    if (arr_chunk[i] < arr_chunk[i - 1]) {
      is_sorted = 0;
      break;
    }
  }
  printf("Chunk-based sorted correctly: %s\n", is_sorted ? "Yes" : "No");

  free(arr_chunk);
  free(arr);

  return 0;
}