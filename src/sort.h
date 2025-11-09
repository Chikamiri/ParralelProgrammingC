#ifndef SORT_H
#define SORT_H

#include "utils.h"
typedef enum { MERGE_SERIAL = 0, MERGE_TREE } MergeVersion;

// Unified sequential sort
void insertionSort(int arr[], int n, int ascending);

// Parallel Sort Prototypes
void insertionSort_omp(int arr[], int n, int ascending, MergeVersion merge_ver);
void insertionSort_pthread(int arr[], int n, int num_threads, int ascending,
                           MergeVersion merge_ver);

// Merge functions (for MPI use)
void merge_serial(int arr[], int l, int m, int r, int ascending);
void merge_tree(int arr[], int chunk_starts[], int chunk_sizes[],
                int num_chunks, int ascending);

// Legacy functions for compatibility
void insertionSortAscending(int arr[], int n);
void insertionSortDescending(int arr[], int n);

#endif // SORT_H
