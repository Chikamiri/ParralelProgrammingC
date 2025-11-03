#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_array(int *arr, int n) {
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    arr[i] = rand() % 1000; // Generate numbers between 0 and 999
  }
}

void print_array(int *arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void copy_array(int *source, int *dest, int n) {
  for (int i = 0; i < n; i++) {
    dest[i] = source[i];
  }
}

double get_time() { return (double)clock() / CLOCKS_PER_SEC; }
