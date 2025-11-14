#include "stevensFileLib.hpp"
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <random>

namespace fs = std::filesystem;

// ============================================================================
// Test Data Setup
// ============================================================================

class FileOperationsBenchmark
{
public:
    static void SetupTestFiles()
    {
        fs::create_directories("benchmark_data");

        // Create small file (100 lines)
        CreateTestFile("benchmark_data/small.txt", 100);

        // Create medium file (10,000 lines)
        CreateTestFile("benchmark_data/medium.txt", 10000);

        // Create large file (1,000,000 lines)
        CreateTestFile("benchmark_data/large.txt", 1000000);

        // Create file with integers
        CreateIntFile("benchmark_data/integers.txt", 10000);

        // Create directory with many files
        for (int i = 0; i < 1000; ++i)
        {
            std::ofstream file("benchmark_data/file_" + std::to_string(i) + ".txt");
            file << "test content";
        }

        // Create mixed extension directory
        for (int i = 0; i < 100; ++i)
        {
            std::ofstream txt("benchmark_data/doc_" + std::to_string(i) + ".txt");
            std::ofstream cpp("benchmark_data/src_" + std::to_string(i) + ".cpp");
            std::ofstream hpp("benchmark_data/hdr_" + std::to_string(i) + ".hpp");
            txt << "content";
            cpp << "content";
            hpp << "content";
        }
    }

    static void CleanupTestFiles()
    {
        if (fs::exists("benchmark_data"))
            fs::remove_all("benchmark_data");
    }

private:
    static void CreateTestFile(const std::string& filename, size_t lineCount)
    {
        std::ofstream file(filename);
        for (size_t i = 0; i < lineCount; ++i)
        {
            file << "This is line number " << i << " with some content to make it realistic\n";
        }
    }

    static void CreateIntFile(const std::string& filename, size_t count)
    {
        std::ofstream file(filename);
        for (size_t i = 0; i < count; ++i)
        {
            file << i << " ";
        }
    }
};

// ============================================================================
// Benchmarks for loadFileIntoVector
// ============================================================================

static void BM_LoadFileIntoVector_SmallFile(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto lines = stevensFileLib::loadFileIntoVector("benchmark_data/small.txt");
        benchmark::DoNotOptimize(lines);
    }
}
BENCHMARK(BM_LoadFileIntoVector_SmallFile);

static void BM_LoadFileIntoVector_MediumFile(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto lines = stevensFileLib::loadFileIntoVector("benchmark_data/medium.txt");
        benchmark::DoNotOptimize(lines);
    }
}
BENCHMARK(BM_LoadFileIntoVector_MediumFile);

static void BM_LoadFileIntoVector_LargeFile(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto lines = stevensFileLib::loadFileIntoVector("benchmark_data/large.txt");
        benchmark::DoNotOptimize(lines);
    }
}
BENCHMARK(BM_LoadFileIntoVector_LargeFile);

static void BM_LoadFileIntoVector_WithFiltering(benchmark::State& state)
{
    std::unordered_map<std::string, std::vector<std::string>> settings;
    settings["skip if starts with"] = {"#", "//"};
    settings["skip if contains"] = {"SKIP"};

    for (auto _ : state)
    {
        auto lines = stevensFileLib::loadFileIntoVector("benchmark_data/medium.txt", settings);
        benchmark::DoNotOptimize(lines);
    }
}
BENCHMARK(BM_LoadFileIntoVector_WithFiltering);

// ============================================================================
// Benchmarks for loadFileIntoVectorOfInts
// ============================================================================

static void BM_LoadFileIntoVectorOfInts(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto numbers = stevensFileLib::loadFileIntoVectorOfInts("benchmark_data/integers.txt");
        benchmark::DoNotOptimize(numbers);
    }
}
BENCHMARK(BM_LoadFileIntoVectorOfInts);

// ============================================================================
// Benchmarks for appendToFile
// ============================================================================

static void BM_AppendToFile(benchmark::State& state)
{
    std::string testFile = "benchmark_data/append_test.txt";
    {
        std::ofstream createFile(testFile);
    } // Create empty file

    for (auto _ : state)
    {
        stevensFileLib::appendToFile(testFile, "test line\n");
    }

    fs::remove(testFile);
}
BENCHMARK(BM_AppendToFile);

static void BM_AppendToFile_LargeContent(benchmark::State& state)
{
    std::string testFile = "benchmark_data/append_large_test.txt";
    {
        std::ofstream createFile(testFile);
    } // Create empty file

    std::string largeContent(10000, 'x');
    largeContent += "\n";

    for (auto _ : state)
    {
        stevensFileLib::appendToFile(testFile, largeContent);
    }

    fs::remove(testFile);
}
BENCHMARK(BM_AppendToFile_LargeContent);

// ============================================================================
// Benchmarks for getRandomFileLine
// ============================================================================

static void BM_GetRandomFileLine_SmallFile(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto line = stevensFileLib::getRandomFileLine("benchmark_data/small.txt");
        benchmark::DoNotOptimize(line);
    }
}
BENCHMARK(BM_GetRandomFileLine_SmallFile);

static void BM_GetRandomFileLine_MediumFile(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto line = stevensFileLib::getRandomFileLine("benchmark_data/medium.txt");
        benchmark::DoNotOptimize(line);
    }
}
BENCHMARK(BM_GetRandomFileLine_MediumFile);

// ============================================================================
// Benchmarks for listFiles
// ============================================================================

static void BM_ListFiles_NoFilter(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto files = stevensFileLib::listFiles("benchmark_data");
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK(BM_ListFiles_NoFilter);

static void BM_ListFiles_WithTargetExtension(benchmark::State& state)
{
    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".txt";

    for (auto _ : state)
    {
        auto files = stevensFileLib::listFiles("benchmark_data", settings);
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK(BM_ListFiles_WithTargetExtension);

static void BM_ListFiles_WithExcludeExtension(benchmark::State& state)
{
    std::unordered_map<std::string, std::string> settings;
    settings["excludeFileExtensions"] = ".cpp,.hpp";

    for (auto _ : state)
    {
        auto files = stevensFileLib::listFiles("benchmark_data", settings);
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK(BM_ListFiles_WithExcludeExtension);

static void BM_ListFiles_WithMultipleFilters(benchmark::State& state)
{
    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".txt,.cpp";
    settings["excludeFiles"] = "file_0.txt,file_1.txt";

    for (auto _ : state)
    {
        auto files = stevensFileLib::listFiles("benchmark_data", settings);
        benchmark::DoNotOptimize(files);
    }
}
BENCHMARK(BM_ListFiles_WithMultipleFilters);

// ============================================================================
// Main function with setup and teardown
// ============================================================================

int main(int argc, char** argv)
{
    FileOperationsBenchmark::SetupTestFiles();

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    FileOperationsBenchmark::CleanupTestFiles();

    return 0;
}
