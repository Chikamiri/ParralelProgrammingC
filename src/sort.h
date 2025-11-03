#ifndef SORT_H
#define SORT_H

#include "utils.h"

// Sequential Sort
void insertionSortAscending(int arr[], int n);
void insertionSortDescending(int arr[], int n);

// Parallel Sort Prototypes
void insertionSort_omp(int arr[], int n);
void insertionSort_pthread(int arr[], int n, int num_threads);
void insertionSort_mpi(int arr[], int n);

#endif // SORT_H
