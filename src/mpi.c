#include "sort.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int *temp_arr, int left, int mid, int right) {
  int i = left, j = mid + 1, k = left;
  while (i <= mid && j <= right) {
    if (arr[i] <= arr[j])
      temp_arr[k++] = arr[i++];
    else
      temp_arr[k++] = arr[j++];
  }
  while (i <= mid)
    temp_arr[k++] = arr[i++];
  while (j <= right)
    temp_arr[k++] = arr[j++];
  for (i = left; i <= right; i++)
    arr[i] = temp_arr[i];
}

void merge_sort(int *arr, int *temp_arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    merge_sort(arr, temp_arr, left, mid);
    merge_sort(arr, temp_arr, mid + 1, right);
    merge(arr, temp_arr, left, mid, right);
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc != 2) {
    if (rank == 0)
      fprintf(stderr, "Usage: mpiexec -n <p> %s <array_size>\n", argv[0]);
    MPI_Finalize();
    return 1;
  }

  int n = atoi(argv[1]);
  if (n <= 0) {
    if (rank == 0)
      fprintf(stderr, "Array size must be a positive integer.\n");
    MPI_Finalize();
    return 1;
  }

  int *global_arr = NULL;
  if (rank == 0) {
    global_arr = (int *)malloc(n * sizeof(int));
    generate_array(global_arr, n);
  }

  int local_n = n / size;
  int *local_arr = (int *)malloc(local_n * sizeof(int));

  MPI_Scatter(global_arr, local_n, MPI_INT, local_arr, local_n, MPI_INT, 0,
              MPI_COMM_WORLD);

  double start_time = MPI_Wtime();

  insertionSortAscending(local_arr, local_n);

  int *gathered_arr = NULL;
  if (rank == 0) {
    gathered_arr = (int *)malloc(n * sizeof(int));
  }

  MPI_Gather(local_arr, local_n, MPI_INT, gathered_arr, local_n, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    int *temp_arr = (int *)malloc(n * sizeof(int));
    merge_sort(gathered_arr, temp_arr, 0, n - 1);
    double end_time = MPI_Wtime();

    printf("MPI Insertion Sort\n");
    printf("Array size: %d\n", n);
    printf("Processes: %d\n", size);
    printf("Execution time: %f seconds\n", end_time - start_time);

    free(temp_arr);
    free(gathered_arr);
    free(global_arr);
  }

  free(local_arr);
  MPI_Finalize();
  return 0;
}
