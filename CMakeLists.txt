# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(LibraryManagementSystem)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

# Include directories
include_directories("${CMAKE_SOURCE_DIR}/include")

# Add executable
add_executable(LMS ${SOURCES})

# Link libraries if needed (uncomment and modify if required)
# target_link_libraries(${PROJECT_NAME} <library_name>)