cmake_minimum_required(VERSION 3.16)
project(stevensFileLib LANGUAGES CXX)

add_library(stevensFileLib INTERFACE)
add_library(stevensFileLib::stevensFileLib ALIAS stevensFileLib)

target_include_directories(stevensFileLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(stevensFileLib INTERFACE cxx_std_17)

option(STEVENSFILELIB_BUILD_TESTS "Build tests" OFF)
option(STEVENSFILELIB_BUILD_BENCHMARKS "Build benchmarks" OFF)

if(STEVENSFILELIB_BUILD_TESTS OR STEVENSFILELIB_BUILD_BENCHMARKS)
    include(FetchContent)
endif()

if(STEVENSFILELIB_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(STEVENSFILELIB_BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(benchmark)
    add_subdirectory(benchmarks)
endif()
