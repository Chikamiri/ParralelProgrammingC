#include "sort.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Cấu trúc để truyền dữ liệu cho mỗi luồng.
// Bao gồm con trỏ tới đoạn mảng và kích thước của đoạn đó.
typedef struct {
    int *arr;
    int size;
} thread_arg_t;

// Hàm được mỗi luồng thực thi.
// Nhận dữ liệu và gọi hàm sắp xếp tuần tự trên đoạn mảng của nó.
void *sort_worker(void *arg) {
    thread_arg_t *data = (thread_arg_t *)arg;
    insertionSortAscending(data->arr, data->size);
    pthread_exit(NULL);
}

// Hàm để trộn hai mảng con đã được sắp xếp.
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Tạo các mảng tạm để chứa hai nửa.
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Trộn các mảng tạm trở lại vào mảng chính.
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Sao chép các phần tử còn lại.
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

// Hàm sắp xếp song song chính.
void insertionSort_pthread(int arr[], int n, int num_threads) {
  
  pthread_t threads[num_threads];
  thread_arg_t args[num_threads];
  int chunk_size = n / num_threads;
  int remainder = n % num_threads;
  int start = 0;

  //1: Chia mảng và tạo các luồng để sắp xếp từng phần.
  for (int i = 0; i < num_threads; i++) {
    int size = chunk_size + (i < remainder ? 1 : 0); // Xử lý mảng lẻ
    args[i].arr = &arr[start];
    args[i].size = size;
    pthread_create(&threads[i], NULL, sort_worker, &args[i]);
    start += size;
  }

  //2: Đợi tất cả các luồng hoàn thành công việc.
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  //3: Trộn các đoạn đã sắp xếp lại với nhau.
  start = 0;
  for (int i = 0; i < num_threads - 1; i++) {
    int size1 = chunk_size + (i < remainder ? 1 : 0);
    int mid = start + size1 - 1;
    int size2 = chunk_size + (i + 1 < remainder ? 1 : 0);
    int end = mid + size2;
    if (i + 1 < num_threads) {
        // Trộn đoạn đã trộn trước đó với đoạn tiếp theo.
        merge(arr, 0, mid, end);
    }
    start = end + 1;
  }
}


int main(int argc, char *argv[]) {
  // Xử lý tham số đầu vào từ dòng lệnh.
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <array_size> <num_threads>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  int p = atoi(argv[2]);

  if (n <= 0 || p <= 0) {
    fprintf(stderr,
            "Array size and number of threads must be positive integers.\n");
    return 1;
  }

  // Khởi tạo mảng và điền dữ liệu ngẫu nhiên.
  int *arr = (int *)malloc(n * sizeof(int));
  generate_array(arr, n);

  printf("Pthreads Insertion Sort\n");
  printf("Array size: %d\n", n);
  printf("Threads: %d\n", p);

  // Đo thời gian và chạy hàm sắp xếp song song.
  double start_time = get_time();
  insertionSort_pthread(arr, n, p);
  double end_time = get_time();

  printf("Execution time: %f seconds\n", end_time - start_time);

  free(arr);
  return 0;
}

// gcc -o ../pthread_sort.exe pthread.c sort.c utils.c -lpthread