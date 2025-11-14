add_executable(stevensFileLib_benchmarks
    benchmark_file_operations.cpp
)

target_link_libraries(stevensFileLib_benchmarks
    PRIVATE
        stevensFileLib
        benchmark::benchmark
        benchmark::benchmark_main
)
