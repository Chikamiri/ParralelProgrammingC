#include "sort.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc != 4) {
    if (rank == 0)
      fprintf(stderr,
              "Usage: mpiexec -n <p> %s <array_size> <mode> <merge_version>\n",
              argv[0]);
    MPI_Finalize();
    return 1;
  }

  int n = atoi(argv[1]);
  int ascending = atoi(argv[2]);
  int merge_v = atoi(argv[3]);

  if (n <= 0 || (ascending != 0 && ascending != 1) || merge_v < 0 ||
      merge_v > 2) {
    if (rank == 0)
      fprintf(stderr, "Invalid arguments.\n");
    MPI_Finalize();
    return 1;
  }

  MergeVersion merge_ver = (MergeVersion)merge_v;

  int *global_arr = NULL;
  if (rank == 0) {
    global_arr = (int *)malloc(n * sizeof(int));
    generate_array(global_arr, n);
  }

  int base = n / size;
  int extra = n % size;
  int *send_counts = (int *)malloc(size * sizeof(int));
  int *displs = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
    send_counts[i] = base + (i < extra ? 1 : 0);

  displs[0] = 0;
  for (int i = 1; i < size; i++)
    displs[i] = displs[i - 1] + send_counts[i - 1];

  int local_n = send_counts[rank];
  int *local_arr = (int *)malloc(local_n * sizeof(int));

  MPI_Scatterv(global_arr, send_counts, displs, MPI_INT, local_arr, local_n,
               MPI_INT, 0, MPI_COMM_WORLD);

  double start_time = MPI_Wtime();
  insertionSort(local_arr, local_n, ascending);

  int *gathered_arr = NULL;
  if (rank == 0)
    gathered_arr = (int *)malloc(n * sizeof(int));

  MPI_Gatherv(local_arr, local_n, MPI_INT, gathered_arr, send_counts, displs,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    int *temp_arr = (int *)malloc(n * sizeof(int));

    double merge_start = MPI_Wtime();
    if (merge_ver == 0)
      merge_sort_serial(gathered_arr, temp_arr, 0, n - 1, ascending);
    else if (merge_ver == 1)
      merge_sort_parallel(gathered_arr, temp_arr, 0, n - 1, ascending);
    else
      merge_sort_tree(gathered_arr, temp_arr, 0, n - 1, ascending);
    double merge_end = MPI_Wtime();

    double end_time = MPI_Wtime();
    printf("MPI Insertion Sort\n");
    printf("Array size: %d\n", n);
    printf("Processes: %d\n", size);
    printf("Merge version: %s\n", merge_ver == 0   ? "Serial"
                                  : merge_ver == 1 ? "Parallel"
                                                   : "Tree");
    printf("Total execution time (sort+merge): %f seconds\n",
           end_time - start_time);
    printf("Merge time only: %f seconds\n", merge_end - merge_start);

    free(temp_arr);
    free(gathered_arr);
    free(global_arr);
  }

  free(send_counts);
  free(displs);
  free(local_arr);

  MPI_Finalize();
  return 0;
}
