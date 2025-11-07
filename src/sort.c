#include "sort.h"
#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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

// ====================== OMP VERSION ====================== //
void insertionSort_omp(int arr[], int n, int ascending) {
  int num_threads = omp_get_max_threads();
  int chunk = n / num_threads;

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int start = tid * chunk;
    int end = (tid == num_threads - 1) ? n : start + chunk;

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

  for (int k = chunk; k < n; k++) {
    int key = arr[k];
    int j = k - 1;
    while (j >= 0 &&
           ((ascending && arr[j] > key) || (!ascending && arr[j] < key))) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// ====================== PTHREAD VERSION ====================== //
static int pthread_ascending; // global flag for compare

typedef struct {
  int *arr;
  int size;
} thread_arg_t;

void *sort_worker(void *arg) {
  thread_arg_t *data = (thread_arg_t *)arg;
  insertionSort(data->arr, data->size, pthread_ascending);
  pthread_exit(NULL);
}

static void merge_arrays(int arr[], int l, int m, int r) {
  int n1 = m - l + 1, n2 = r - m;
  int *L = malloc(n1 * sizeof(int));
  int *R = malloc(n2 * sizeof(int));

  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  int i = 0, j = 0, k = l;

  while (i < n1 && j < n2) {
    if ((pthread_ascending && L[i] <= R[j]) ||
        (!pthread_ascending && L[i] >= R[j]))
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

void insertionSort_pthread(int arr[], int n, int num_threads, int ascending) {
  pthread_ascending = ascending;

  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_arg_t *args = malloc(num_threads * sizeof(thread_arg_t));

  int chunk = n / num_threads;
  int extra = n % num_threads;
  int start = 0;

  for (int i = 0; i < num_threads; i++) {
    int size = chunk + (i < extra ? 1 : 0);
    args[i].arr = &arr[start];
    args[i].size = size;
    pthread_create(&threads[i], NULL, sort_worker, &args[i]);
    start += size;
  }

  for (int i = 0; i < num_threads; i++)
    pthread_join(threads[i], NULL);

  int merged = chunk + (extra > 0 ? 1 : 0);
  for (int i = 1; i < num_threads; i++) {
    int size = chunk + (i < extra ? 1 : 0);
    merge_arrays(arr, 0, merged - 1, merged + size - 1);
    merged += size;
  }

  free(threads);
  free(args);
}
