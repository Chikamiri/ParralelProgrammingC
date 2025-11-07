@echo off
echo Running benchmark inside Ubuntu...

set WIN_PATH=%CD%
for /f "delims=" %%i in ('wsl -d Ubuntu wslpath "%WIN_PATH%"') do set WSL_PATH=%%i

wsl -d Ubuntu fish -c "cd '%WSL_PATH%' && fish ./benchmark.sh"
