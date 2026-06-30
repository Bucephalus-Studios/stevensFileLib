add_executable(benchmarks
    benchmark_file_operations.cpp
)

target_link_libraries(benchmarks
    PRIVATE
        stevensFileLib
        benchmark::benchmark_main
)