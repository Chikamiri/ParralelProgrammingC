#!/bin/bash

ARRAY_SIZE=100000
THREAD_COUNTS="2 4 6 8 10 12"
MERGE_VERSIONS="0 1 2"
MPI_PROCESSES="2 4 6 8 10 12"
RESULTS_FILE="results_linux.csv"
LOG_DIR="logs"
BUILD_DIR="build"

declare -A MERGE_NAME
MERGE_NAME[0]="Serial"
MERGE_NAME[1]="Tree"

echo "=== Rebuild Project ==="
rm -rf ${BUILD_DIR} ${LOG_DIR}
mkdir -p ${BUILD_DIR} ${LOG_DIR}
cd ${BUILD_DIR}
cmake ..
make -j$(nproc)
cd ..

echo "Version,Threads,ArraySize,MergeVersion,MergeTime,TotalTime" > ${RESULTS_FILE}

# ====================== Sequential ======================
echo "=== Sequential ==="
SEQ_LOG="${LOG_DIR}/seq.log"
./${BUILD_DIR}/seq ${ARRAY_SIZE} > ${SEQ_LOG} 2>&1
SORT_TIME=$(grep "Execution time" ${SEQ_LOG} | awk '{print $3}')
TOTAL_TIME=$(grep "Total execution time" ${SEQ_LOG} | awk '{print $5}')
echo "Sequential,1,${ARRAY_SIZE},NA,${SORT_TIME},${TOTAL_TIME}" >> ${RESULTS_FILE}

# ====================== OpenMP ======================
echo "=== OpenMP ==="
for p in ${THREAD_COUNTS}; do
    LOG_FILE="${LOG_DIR}/omp-${p}.log"
    > ${LOG_FILE}

    for m in ${MERGE_VERSIONS}; do
        echo "Merge version: ${MERGE_NAME[$m]}" >> ${LOG_FILE}
        ./${BUILD_DIR}/omp ${ARRAY_SIZE} ${p} ${m} >> ${LOG_FILE} 2>&1
        echo "" >> ${LOG_FILE}   # xuống dòng
        MERGE_TIME=$(grep "Merge time" ${LOG_FILE} | tail -n1 | awk '{print $3}')
        TOTAL_TIME=$(grep "Total execution time" ${LOG_FILE} | tail -n1 | awk '{print $5}')
        echo "OpenMP,${p},${ARRAY_SIZE},${MERGE_NAME[$m]},${MERGE_TIME},${TOTAL_TIME}" >> ${RESULTS_FILE}
    done
done

# ====================== Pthreads ======================
echo "=== Pthreads ==="
for p in ${THREAD_COUNTS}; do
    LOG_FILE="${LOG_DIR}/pthread-${p}.log"
    > ${LOG_FILE}

    for m in ${MERGE_VERSIONS}; do
        echo "Merge version: ${MERGE_NAME[$m]}" >> ${LOG_FILE}
        ./${BUILD_DIR}/pthread ${ARRAY_SIZE} ${p} 1 ${m} >> ${LOG_FILE} 2>&1
        echo "" >> ${LOG_FILE}   # xuống dòng
        MERGE_TIME=$(grep "Merge time" ${LOG_FILE} | tail -n1 | awk '{print $3}')
        TOTAL_TIME=$(grep "Total execution time" ${LOG_FILE} | tail -n1 | awk '{print $5}')
        echo "Pthreads,${p},${ARRAY_SIZE},${MERGE_NAME[$m]},${MERGE_TIME},${TOTAL_TIME}" >> ${RESULTS_FILE}
    done
done

# ====================== MPI ======================
echo "=== MPI ==="
for p in ${MPI_PROCESSES}; do
    LOG_FILE="${LOG_DIR}/mpi-${p}.log"
    > ${LOG_FILE}

    for m in ${MERGE_VERSIONS}; do
        echo "Merge version: ${MERGE_NAME[$m]}" >> ${LOG_FILE}
        mpiexec --oversubscribe -n ${p} ./${BUILD_DIR}/mpi ${ARRAY_SIZE} 1 ${m} >> ${LOG_FILE} 2>&1
        echo "" >> ${LOG_FILE}
        TOTAL_TIME=$(grep "Total execution time" ${LOG_FILE} | tail -n1 | awk '{print $5}')
        echo "MPI,${p},${ARRAY_SIZE},${MERGE_NAME[$m]},,${TOTAL_TIME}" >> ${RESULTS_FILE}
    done
done

echo "Done. Results saved to ${RESULTS_FILE}"
echo "All logs saved to ${LOG_DIR}"
