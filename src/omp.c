#include "sort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Array size must be positive integer\n");
        return 1;
    }

    // Generate random array
    int *arr = malloc(n * sizeof(int));
    generate_array(arr, n);

    printf("Array size: %d\n", n);
    printf("Threads: 2, 4, 6, 8, 10, 12\n\n");

    // Test sequential
    int *arr_seq_asc = malloc(n * sizeof(int));
    int *arr_seq_desc = malloc(n * sizeof(int));

    copy_array(arr, arr_seq_asc, n);
    double start = get_time();
    insertionSort(arr_seq_asc, n, 1);
    double seq_asc_time = get_time() - start;

    copy_array(arr, arr_seq_desc, n);
    start = get_time();
    insertionSort(arr_seq_desc, n, 0);
    double seq_desc_time = get_time() - start;

    printf("SEQUENTIAL:\n");
    printf("Ascending:  %.6f seconds\n", seq_asc_time);
    printf("Descending: %.6f seconds\n", seq_desc_time);

    // Test parallel with different thread counts
    printf("\nPARALLEL (OpenMP):\n");
    printf("%-8s %-12s %-10s %-12s\n",
           "Threads", "Time(s)", "Speedup", "Efficiency");
    printf("%-8s %-12s %-10s %-12s\n",
           "-------", "--------", "-------", "---------");

    int threads_list[] = {2, 4, 6, 8, 10, 12};

    for (int i = 0; i < 6; i++) {
        int p = threads_list[i];
        omp_set_num_threads(p);

        int *arr_par_asc = malloc(n * sizeof(int));
        int *arr_par_desc = malloc(n * sizeof(int));

        copy_array(arr, arr_par_asc, n);
        start = get_time();
        insertionSort_omp(arr_par_asc, n, 1);
        double par_asc_time = get_time() - start;

        copy_array(arr, arr_par_desc, n);
        start = get_time();
        insertionSort_omp(arr_par_desc, n, 0);
        double par_desc_time = get_time() - start;

        double avg_par_time = (par_asc_time + par_desc_time) / 2;
        double avg_seq_time = (seq_asc_time + seq_desc_time) / 2;
        double speedup = avg_seq_time / avg_par_time;
        double efficiency = (speedup / p) * 100;

        printf("%-8d %-12.6f %-10.2f %-11.1f%%\n",
               p, avg_par_time, speedup, efficiency);

        free(arr_par_asc);
        free(arr_par_desc);
    }

    free(arr);
    free(arr_seq_asc);
    free(arr_seq_desc);

    return 0;
}