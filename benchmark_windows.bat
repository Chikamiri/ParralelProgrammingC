@echo off
setlocal enabledelayedexpansion

set ARRAY_SIZE=100000
set THREAD_COUNTS=2 4 6 8 10 12
set RESULTS_FILE=results_windows.csv

echo === Rebuild Project ===
if exist build rmdir /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
cd ..

echo Version,Threads,ArraySize,Time > %RESULTS_FILE%

echo === Sequential ===
for /f "tokens=3" %%a in ('build\seq.exe %ARRAY_SIZE% ^| findstr "Execution"') do (
    set SEQ_TIME=%%a
)
echo Sequential,1,%ARRAY_SIZE%,!SEQ_TIME! >> %RESULTS_FILE%

echo === OpenMP ===
for %%p in (%THREAD_COUNTS%) do (
    for /f "tokens=3" %%t in ('build\omp.exe %ARRAY_SIZE% %%p ^| findstr "Execution"') do (
        echo OpenMP,%%p,%ARRAY_SIZE%,%%t >> %RESULTS_FILE%
    )
)

echo === Pthreads ===
for %%p in (%THREAD_COUNTS%) do (
    for /f "tokens=4" %%t in ('build\pthread.exe %ARRAY_SIZE% %%p 1^| findstr "Execution"') do (
        echo Pthreads,%%p,%ARRAY_SIZE%,%%t >> %RESULTS_FILE%
    )
)

echo Done. Results saved to %RESULTS_FILE%
endlocal
