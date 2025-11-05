#ifndef SORT_H
#define SORT_H

void insertionSort(int arr[], int n, int ascending);
void insertionSort_omp(int arr[], int n, int ascending);
void generate_array(int arr[], int n);
void copy_array(int source[], int dest[], int n);
double get_time();

#endif