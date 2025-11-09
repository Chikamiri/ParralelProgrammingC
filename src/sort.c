#include "sort.h"
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ====================== SEQUENTIAL ====================== //
void insertionSort(int arr[], int n, int ascending) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 &&
           ((ascending && arr[j] > key) || (!ascending && arr[j] < key))) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// ====================== MERGE VERSIONS ====================== //
void merge_serial(int arr[], int l, int m, int r, int ascending) {
  int n1 = m - l + 1, n2 = r - m;
  int *L = malloc(n1 * sizeof(int));
  int *R = malloc(n2 * sizeof(int));
  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int i = 0; i < n2; i++)
    R[i] = arr[m + 1 + i];
  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if ((ascending && L[i] <= R[j]) || (!ascending && L[i] >= R[j]))
      arr[k++] = L[i++];
    else
      arr[k++] = R[j++];
  }
  while (i < n1)
    arr[k++] = L[i++];
  while (j < n2)
    arr[k++] = R[j++];
  free(L);
  free(R);
}

void merge_tree(int arr[], int chunk_starts[], int chunk_sizes[],
                int num_chunks, int ascending) {
  int *temp = malloc(sizeof(int) * (chunk_starts[num_chunks - 1] +
                                    chunk_sizes[num_chunks - 1]));
  int cur_chunks = num_chunks;
  while (cur_chunks > 1) {
#pragma omp parallel for
    for (int i = 0; i < cur_chunks / 2; i++) {
      int l = chunk_starts[2 * i];
      int m = l + chunk_sizes[2 * i] - 1;
      int r = m + chunk_sizes[2 * i + 1];
      merge_serial(arr, l, m, r, ascending);
    }
    for (int i = 0; i < cur_chunks / 2; i++) {
      chunk_sizes[i] = chunk_sizes[2 * i] + chunk_sizes[2 * i + 1];
      chunk_starts[i] = chunk_starts[2 * i];
    }
    if (cur_chunks % 2 == 1) {
      chunk_sizes[cur_chunks / 2] = chunk_sizes[cur_chunks - 1];
      chunk_starts[cur_chunks / 2] = chunk_starts[cur_chunks - 1];
      cur_chunks = cur_chunks / 2 + 1;
    } else
      cur_chunks /= 2;
  }
  free(temp);
}

// ====================== OMP VERSION ====================== //
void insertionSort_omp(int arr[], int n, int ascending,
                       MergeVersion merge_ver) {
  int num_threads = omp_get_max_threads();
  int chunk = n / num_threads;
  int extra = n % num_threads;

  int *chunk_starts = malloc(num_threads * sizeof(int));
  int *chunk_sizes = malloc(num_threads * sizeof(int));

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int start = tid * chunk + (tid < extra ? tid : extra);
    int size = chunk + (tid < extra ? 1 : 0);
    int end = start + size;
    chunk_starts[tid] = start;
    chunk_sizes[tid] = size;

    for (int i = start + 1; i < end; i++) {
      int key = arr[i];
      int j = i - 1;
      while (j >= start &&
             ((ascending && arr[j] > key) || (!ascending && arr[j] < key))) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = key;
    }
  }

  // Merge
  double t_merge_start = get_time();
  switch (merge_ver) {
  case MERGE_SERIAL:
    for (int i = 1; i < num_threads; i++)
      merge_serial(arr, 0, chunk_starts[i] - 1,
                   chunk_starts[i] + chunk_sizes[i] - 1, ascending);
    break;
  case MERGE_TREE:
    merge_tree(arr, chunk_starts, chunk_sizes, num_threads, ascending);
    break;
  }
  double t_merge_end = get_time();
  printf("Merge time: %f seconds\n", t_merge_end - t_merge_start);

  free(chunk_starts);
  free(chunk_sizes);
}

// ====================== PTHREAD VERSION ====================== //
static int pthread_ascending;

typedef struct {
  int *arr;
  int size;
} thread_arg_t;

void *sort_worker(void *arg) {
  thread_arg_t *data = (thread_arg_t *)arg;
  insertionSort(data->arr, data->size, pthread_ascending);
  pthread_exit(NULL);
}

void insertionSort_pthread(int arr[], int n, int num_threads, int ascending,
                           MergeVersion merge_ver) {
  pthread_ascending = ascending;
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_arg_t *args = malloc(num_threads * sizeof(thread_arg_t));
  int chunk = n / num_threads;
  int extra = n % num_threads;
  int *chunk_starts = malloc(num_threads * sizeof(int));
  int *chunk_sizes = malloc(num_threads * sizeof(int));
  int start = 0;

  for (int i = 0; i < num_threads; i++) {
    int size = chunk + (i < extra ? 1 : 0);
    args[i].arr = &arr[start];
    args[i].size = size;
    chunk_starts[i] = start;
    chunk_sizes[i] = size;
    pthread_create(&threads[i], NULL, sort_worker, &args[i]);
    start += size;
  }

  for (int i = 0; i < num_threads; i++)
    pthread_join(threads[i], NULL);

  // Merge
  double t_merge_start = get_time();
  switch (merge_ver) {
  case MERGE_SERIAL:
    for (int i = 1; i < num_threads; i++)
      merge_serial(arr, 0, chunk_starts[i] - 1,
                   chunk_starts[i] + chunk_sizes[i] - 1, ascending);
    break;
  case MERGE_TREE:
    merge_tree(arr, chunk_starts, chunk_sizes, num_threads, ascending);
    break;
  }
  double t_merge_end = get_time();
  printf("Merge time: %f seconds\n", t_merge_end - t_merge_start);

  free(chunk_starts);
  free(chunk_sizes);
  free(threads);
  free(args);
}
