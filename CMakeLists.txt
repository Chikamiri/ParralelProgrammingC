cmake_minimum_required(VERSION 3.10)
project(insertion_sort C)

set(CMAKE_C_STANDARD 11)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)

add_library(sort STATIC
    ${SRC_DIR}/sort.c
    ${SRC_DIR}/utils.c
)
target_include_directories(sort PUBLIC ${SRC_DIR})
target_link_libraries(sort PUBLIC OpenMP::OpenMP_C)

add_executable(seq ${SRC_DIR}/seq.c)
target_link_libraries(seq PRIVATE sort)

add_executable(omp ${SRC_DIR}/omp.c)
target_link_libraries(omp PRIVATE sort)

add_executable(pthread ${SRC_DIR}/pthread.c)
target_link_libraries(pthread PRIVATE sort Threads::Threads)

if(UNIX)
    find_package(MPI REQUIRED)
    add_executable(mpi ${SRC_DIR}/mpi.c)
    target_link_libraries(mpi PRIVATE sort MPI::MPI_C)
endif()
