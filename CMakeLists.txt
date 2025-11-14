cmake_minimum_required(VERSION 3.14)
project(stevensFileLib VERSION 0.2 LANGUAGES CXX)

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Enable testing
include(CTest)
enable_testing()

# Options
option(BUILD_TESTS "Build the tests" ON)
option(BUILD_BENCHMARKS "Build the benchmarks" ON)

# Fetch Google Test
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()

# Fetch Google Benchmark
if(BUILD_BENCHMARKS)
    include(FetchContent)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)

    add_subdirectory(benchmarks)
endif()

# Installation
install(FILES stevensFileLib.hpp
        DESTINATION include)

install(TARGETS stevensFileLib
        EXPORT stevensFileLibTargets
        INCLUDES DESTINATION include)

install(EXPORT stevensFileLibTargets
        FILE stevensFileLibTargets.cmake
        NAMESPACE stevensFileLib::
        DESTINATION lib/cmake/stevensFileLib)

# Create package config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/stevensFileLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/stevensFileLibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/stevensFileLibConfig.cmake"
    INSTALL_DESTINATION lib/cmake/stevensFileLib
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/stevensFileLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/stevensFileLibConfigVersion.cmake"
    DESTINATION lib/cmake/stevensFileLib
)
