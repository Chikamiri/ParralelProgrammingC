#!/bin/bash

ARRAY_SIZE=100000
THREAD_COUNTS="2 4 6 8 10 12"
MPI_PROCESSES="2 4 6 8 10 12"
RESULTS_FILE="results_linux.csv"
BUILD_DIR="build"

echo "=== Rebuild Project ==="
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

cmake ..
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "\n[ERROR] Build project failed! Please check your source code."
    cd ..
    exit 1
fi
cd ..

echo "Version,Threads,ArraySize,Time" > ${RESULTS_FILE}

echo "=== Sequential (Ascending) ==="
SEQ_TIME=$(./${BUILD_DIR}/seq ${ARRAY_SIZE} | grep "Execution time" | awk '{print $3}')
echo "Sequential,1,${ARRAY_SIZE},${SEQ_TIME}" >> ${RESULTS_FILE}

echo "=== OpenMP ==="
for p in ${THREAD_COUNTS}; do
    export OMP_NUM_THREADS=$p
    OMP_TIME=$(./${BUILD_DIR}/omp ${ARRAY_SIZE} ${p} | grep "Execution time" | awk '{print $3}')
    echo "OpenMP,${p},${ARRAY_SIZE},${OMP_TIME}" >> ${RESULTS_FILE}
done
unset OMP_NUM_THREADS

echo "=== Pthreads ==="
for p in ${THREAD_COUNTS}; do
    PTHREAD_TIME=$(./${BUILD_DIR}/pthread ${ARRAY_SIZE} ${p} 1 | grep "Execution time" | awk '{print $3}')
    echo "Pthreads,${p},${ARRAY_SIZE},${PTHREAD_TIME}" >> ${RESULTS_FILE}
done

echo "=== MPI ==="
for p in ${MPI_PROCESSES}; do
    # mpiexec -n <p> ./mpi <array_size> <mode=1>
    MPI_OUTPUT=$(mpiexec --oversubscribe -n ${p} ./${BUILD_DIR}/mpi ${ARRAY_SIZE} 1)
    MPI_TIME=$(echo "${MPI_OUTPUT}" | grep "Execution time" | awk '{print $3}')
    echo "MPI,${p},${ARRAY_SIZE},${MPI_TIME}" >> ${RESULTS_FILE}
done


echo "Done. Results saved to ${RESULTS_FILE}"