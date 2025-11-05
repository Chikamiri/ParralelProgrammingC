#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
double get_time() {
    LARGE_INTEGER frequency, time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
    return (double)time.QuadPart / frequency.QuadPart;
}
#else
#include <sys/time.h>
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}
#endif

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

// Utility functions
void generate_array(int arr[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

void copy_array(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}