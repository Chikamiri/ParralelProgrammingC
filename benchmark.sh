#!/bin/bash

# Configuration
ARRAY_SIZE=100000
MPI_PROCESSORS="2 4 6 8 10 12"
RESULTS_FILE="results.csv"

# Build the project
echo "Configuring and building the project..."
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
cmake ..
make
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi
cd ..

echo "Running benchmarks..."
echo "Version,Processes,ArraySize,Time" > $RESULTS_FILE

# Run Sequential Version
echo "Running Sequential..."
SEQ_TIME=$(./build/seq $ARRAY_SIZE | grep "Ascending" | awk '{print $4}')
echo "Sequential,1,$ARRAY_SIZE,$SEQ_TIME" >> $RESULTS_FILE

# Run MPI Version
echo "Running MPI..."
for p in $MPI_PROCESSORS; do
    echo "  Processes: $p"
    MPI_TIME=$(mpirun -np $p ./build/mpi $ARRAY_SIZE | grep "Execution" | awk '{print $4}')
    echo "MPI,$p,$ARRAY_SIZE,$MPI_TIME" >> $RESULTS_FILE
done

echo "Benchmarking complete. Results saved to $RESULTS_FILE"
