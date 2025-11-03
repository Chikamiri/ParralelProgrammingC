@echo off
setlocal

REM Configuration
set ARRAY_SIZE=100000
set MPI_PROCESSORS=2 4 6 8 10 12
set RESULTS_FILE=results.csv

REM Build the project
echo "Configuring the project with CMake..."
if not exist build (
    mkdir build
)
cd build
cmake -G "MinGW Makefiles" ..
if %errorlevel% neq 0 (
    echo "CMake configuration failed."
    exit /b 1
)

echo "Building the project..."
cmake --build .
if %errorlevel% neq 0 (
    echo "Build failed."
    exit /b 1
)
cd ..

echo "Running benchmarks..."
echo "Version,Processes,ArraySize,Time" > %RESULTS_FILE%

REM Run Sequential Version
echo "Running Sequential..."
for /f "tokens=4" %%a in ('build\seq.exe %ARRAY_SIZE% ^| findstr "Ascending"') do (
    set SEQ_TIME=%%a
)
echo "Sequential,1,%ARRAY_SIZE%,%SEQ_TIME%" >> %RESULTS_FILE%


REM Run MPI Version
echo "Running MPI..."
for %%p in (%MPI_PROCESSORS%) do (
    echo "  Processes: %%p"
    for /f "tokens=4" %%a in ('mpiexec -n %%p build\mpi.exe %ARRAY_SIZE% ^| findstr "Execution"') do (
        set MPI_TIME=%%a
    )
    echo "MPI,%%p,%ARRAY_SIZE%,%MPI_TIME%" >> %RESULTS_FILE%
)

echo "Benchmarking complete. Results saved to %RESULTS_FILE%"
endlocal
