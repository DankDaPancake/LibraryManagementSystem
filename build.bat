@echo off

:: Remove the build directory if it exists
IF EXIST build (
    rmdir /s /q build
)

:: Recreate the build directory
mkdir build

:: Navigate to build directory
cd build

:: Configure CMake with MinGW Makefiles generator
cmake -G "MinGW Makefiles" ..

:: Build the project
cmake --build . --config Release --clean-first

:: Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! The executable is in build/LMS.exe
) else (
    echo.
    echo Build failed with error code %ERRORLEVEL%
)

:: Return to original directory
cd ..

pause 