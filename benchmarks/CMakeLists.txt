cmake_minimum_required(VERSION 3.14)
project(stevensFileLib_benchmarks VERSION 0.2 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Header-only library interface
add_library(stevensFileLib INTERFACE)
target_include_directories(stevensFileLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

# Fetch Google Benchmark
include(FetchContent)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Build benchmark executable
add_executable(stevensFileLib_benchmarks
    benchmark_file_operations.cpp
)

target_link_libraries(stevensFileLib_benchmarks
    PRIVATE
        stevensFileLib
        benchmark::benchmark
        benchmark::benchmark_main
)
