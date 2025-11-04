@echo off
setlocal enabledelayedexpansion

REM Configuration
set ARRAY_SIZE=100000
set THREAD_COUNTS=2 4 6 8 10 12
set RESULTS_FILE=results_windows.csv

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
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo "Build failed."
    exit /b 1
)
cd ..

echo "Running benchmarks..."
echo "Version,Threads,ArraySize,Time" > %RESULTS_FILE%

REM Run Sequential Version
echo "Running Sequential..."
for /f "tokens=4" %%a in ('build\Release\seq.exe %ARRAY_SIZE% ^| findstr "Ascending"') do (
    set SEQ_TIME=%%a
)
echo Sequential,1,%ARRAY_SIZE%,!SEQ_TIME! >> %RESULTS_FILE%

REM Run OpenMP Version
echo "Running OpenMP..."
for /f "tokens=2,5" %%p in ('build\Release\omp.exe %ARRAY_SIZE% ^| findstr "Threads"') do (
    set OMP_THREADS=%%p
    set OMP_TIME=%%q
    echo OpenMP,!OMP_THREADS:~0,-1!,%ARRAY_SIZE%,!OMP_TIME! >> %RESULTS_FILE%
)

REM Run Pthreads Version
echo "Running Pthreads..."
for %%p in (%THREAD_COUNTS%) do (
    echo "  Threads: %%p"
    for /f "tokens=4" %%a in ('build\Release\pthread.exe %ARRAY_SIZE% %%p ^| findstr "Execution"') do (
        set PTHREAD_TIME=%%a
    )
    echo Pthreads,%%p,%ARRAY_SIZE%,!PTHREAD_TIME! >> %RESULTS_FILE%
)

echo "Benchmarking complete. Results saved to %RESULTS_FILE%"
endlocal
