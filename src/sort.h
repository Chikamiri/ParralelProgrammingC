#ifndef SORT_H
#define SORT_H

#include "utils.h"

// Unified sequential sort
void insertionSort(int arr[], int n, int ascending);

// Parallel Sort Prototypes
void insertionSort_omp(int arr[], int n, int ascending);
void insertionSort_pthread(int arr[], int n, int num_threads);
void insertionSort_mpi(int arr[], int n);

// Legacy functions for compatibility with older main files
void insertionSortAscending(int arr[], int n);
void insertionSortDescending(int arr[], int n);

#endif // SORT_H
