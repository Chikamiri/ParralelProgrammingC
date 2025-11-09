@echo off
setlocal enabledelayedexpansion

set ARRAY_SIZE=100000
set THREAD_COUNTS=2 4 6 8 10 12
set MERGE_VERSIONS=0 1 2
set RESULTS_FILE=results_windows.csv
set LOG_DIR=logs

set MERGE_NAME_0=Serial
set MERGE_NAME_1=Tree

if exist build rmdir /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
cd ..

if exist logs rmdir /s /q %LOG_DIR%
mkdir %LOG_DIR%

echo Version,Threads,ArraySize,MergeVersion,MergeTime,TotalTime > %RESULTS_FILE%

:: ====================== Sequential ======================
echo === Sequential ===
build\seq.exe %ARRAY_SIZE% > %LOG_DIR%\seq.log 2>&1
for /f "tokens=3" %%a in ('findstr "Execution time" %LOG_DIR%\seq.log') do set SORT_TIME=%%a
for /f "tokens=4" %%b in ('findstr "Total execution time" %LOG_DIR%\seq.log') do set TOTAL_TIME=%%b
echo Sequential,1,%ARRAY_SIZE%,NA,!SORT_TIME!,!TOTAL_TIME! >> %RESULTS_FILE%

:: ====================== OpenMP ======================
echo === OpenMP ===
for %%p in (%THREAD_COUNTS%) do (
    set LOG_FILE=%LOG_DIR%\omp-%%p.log
    if exist !LOG_FILE! del !LOG_FILE!

    for %%m in (%MERGE_VERSIONS%) do (
        echo Merge version: !MERGE_NAME_%%m! >> !LOG_FILE!
        build\omp.exe %ARRAY_SIZE% %%p %%m >> !LOG_FILE! 2>&1
        echo. >> !LOG_FILE!

        for /f "tokens=3" %%s in ('findstr "Merge time" !LOG_FILE!') do set MERGE_TIME=%%s
        for /f "tokens=4" %%t in ('findstr "Total execution time" !LOG_FILE!') do set TOTAL_TIME=%%t

        set MERGE_NAME=!MERGE_NAME_%%m!
        echo OpenMP,%%p,%ARRAY_SIZE%,!MERGE_NAME!,!MERGE_TIME!,!TOTAL_TIME! >> %RESULTS_FILE%
    )
)

:: ====================== Pthreads ======================
echo === Pthreads ===
for %%p in (%THREAD_COUNTS%) do (
    set LOG_FILE=%LOG_DIR%\pthread-%%p.log
    if exist !LOG_FILE! del !LOG_FILE!

    for %%m in (%MERGE_VERSIONS%) do (
        echo Merge version: !MERGE_NAME_%%m! >> !LOG_FILE!
        build\pthread.exe %ARRAY_SIZE% %%p 1 %%m >> !LOG_FILE! 2>&1
        echo. >> !LOG_FILE!

        for /f "tokens=3" %%s in ('findstr "Merge time" !LOG_FILE!') do set MERGE_TIME=%%s
        for /f "tokens=4" %%t in ('findstr "Total execution time" !LOG_FILE!') do set TOTAL_TIME=%%t

        set MERGE_NAME=!MERGE_NAME_%%m!
        echo Pthreads,%%p,%ARRAY_SIZE%,!MERGE_NAME!,!MERGE_TIME!,!TOTAL_TIME! >> %RESULTS_FILE%
    )
)

echo Done. Results saved to %RESULTS_FILE%
echo All logs saved to %LOG_DIR%
endlocal
