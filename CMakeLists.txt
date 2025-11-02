cmake_minimum_required(VERSION 3.10)
project(insertion_sort C)

set(CMAKE_C_STANDARD 11)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Lib
add_library(sort STATIC
    ${SRC_DIR}/sort.c
)
target_include_directories(sort PUBLIC ${SRC_DIR})


# Exec
add_executable(seq ${SRC_DIR}/seq.c)
target_link_libraries(seq PRIVATE sort)

add_executable(omp ${SRC_DIR}/omp.c)
target_link_libraries(omp PRIVATE sort)

add_executable(pthread ${SRC_DIR}/pthread.c)
target_link_libraries(pthread PRIVATE sort)

add_executable(mpi ${SRC_DIR}/mpi.c)
target_link_libraries(mpi PRIVATE sort)


# OpenMP
target_compile_options(omp PRIVATE -fopenmp)
target_link_libraries(omp PRIVATE -fopenmp)


# PThread
target_compile_options(pthread PRIVATE -pthread)
target_link_libraries(pthread PRIVATE -pthread)


# MPI
find_package(MPI REQUIRED)
if(MPI_C_FOUND)
    target_link_libraries(mpi PRIVATE MPI::MPI_C)
endif()
