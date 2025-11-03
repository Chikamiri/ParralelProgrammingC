cmake_minimum_required(VERSION 3.10)
project(insertion_sort C)

set(CMAKE_C_STANDARD 11)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Lib chung
add_library(sort STATIC
    ${SRC_DIR}/sort.c
    ${SRC_DIR}/utils.c
)
target_include_directories(sort PUBLIC ${SRC_DIR})

# Exec không MPI (chạy được trên Win)
add_executable(seq ${SRC_DIR}/seq.c)
target_link_libraries(seq PRIVATE sort)

add_executable(omp ${SRC_DIR}/omp.c)
target_link_libraries(omp PRIVATE sort)

add_executable(pthread ${SRC_DIR}/pthread.c)
target_link_libraries(pthread PRIVATE sort)

# OpenMP
target_compile_options(omp PRIVATE -fopenmp)
target_link_libraries(omp PRIVATE -fopenmp)

# pthread
target_compile_options(pthread PRIVATE -pthread)
target_link_libraries(pthread PRIVATE -pthread)

# ---- Chỉ bật MPI trên Linux/WSL ----
if(UNIX)
    find_package(MPI REQUIRED)
    add_executable(mpi ${SRC_DIR}/mpi.c)
    target_link_libraries(mpi PRIVATE sort MPI::MPI_C)
endif()
