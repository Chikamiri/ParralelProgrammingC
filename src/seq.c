#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int N;
  printf("Nhap so luong phan tu N: ");
  scanf("%d", &N);

  int arr[N];
  srand(time(NULL));
  for (int i = 0; i < N; i++)
    arr[i] = rand() % 100;

  printf("\nMang ban dau: ");
  printArray(arr, N);

  insertionSortAscending(arr, N);
  printf("\nMang sap xep tang dan: ");
  printArray(arr, N);

  insertionSortDescending(arr, N);
  printf("\nMang sap xep giam dan: ");
  printArray(arr, N);

  return 0;
}
