#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Sequential insertion sort
void insertionSort(int arr[], int n, int ascending) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    if (ascending) {
      while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
    } else {
      while (j >= 0 && arr[j] < key) {
        arr[j + 1] = arr[j];
        j--;
      }
    }
    arr[j + 1] = key;
  }
}

// Parallel insertion sort using OpenMP
void insertionSort_omp(int arr[], int n, int ascending) {
#pragma omp parallel for
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    if (ascending) {
      while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
    } else {
      while (j >= 0 && arr[j] < key) {
        arr[j + 1] = arr[j];
        j--;
      }
    }
    arr[j + 1] = key;
  }
}
