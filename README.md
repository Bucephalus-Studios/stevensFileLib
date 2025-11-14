# stevensFileLib

A modern, header-only C++17 library for simplified file and directory operations.

## Features

- **Header-only**: No compilation required, just include and use
- **Modern C++17**: Uses `<filesystem>`, `<random>`, and other modern C++ features
- **Comprehensive**: File reading, writing, and directory listing with filtering
- **Well-tested**: Extensive unit tests with Google Test
- **Performant**: Optimized operations with benchmarks using Google Benchmark
- **Clean API**: Simple, readable functions with structured configuration
- **Self-contained**: No external dependencies (removed stevensStringLib dependency)

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14+ (for building tests and benchmarks)

## Quick Start

### Using as Header-Only Library

Simply copy `stevensFileLib.hpp` to your project and include it:

```cpp
#include "stevensFileLib.hpp"

int main()
{
    // Write to a file
    stevensFileLib::appendToFile("output.txt", "Hello, World!\n");

    // Read file into vector
    auto lines = stevensFileLib::loadFileIntoVector("input.txt");

    // List files in directory
    auto files = stevensFileLib::listFiles("./data");

    return 0;
}
```

## Building Tests and Benchmarks

### Configure and Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run Tests

```bash
ctest --output-on-failure
# or directly
./stevensFileLib_tests
```

### Run Benchmarks

```bash
./stevensFileLib_benchmarks
```

### Build Options

- `BUILD_TESTS`: Build unit tests (default: ON)
- `BUILD_BENCHMARKS`: Build benchmarks (default: ON)

```bash
cmake -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=OFF ..
```

## API Reference

### File Opening

#### `openInputFile`
```cpp
std::ifstream openInputFile(const std::string& filePath)
```
Opens a file for reading with validation.

**Throws**: `std::invalid_argument` if file cannot be opened

**Example**:
```cpp
auto file = stevensFileLib::openInputFile("data.txt");
std::string line;
while (std::getline(file, line))
{
    // Process line
}
```

#### `openOutputFile`
```cpp
std::ofstream openOutputFile(const std::string& filePath)
```
Opens a file for appending with validation.

**Throws**: `std::invalid_argument` if file cannot be opened

### File Writing

#### `appendToFile`
```cpp
template<typename ContentType>
void appendToFile(const std::string& filePath, const ContentType& content,
                 bool createIfNonExistent = true)
```
Appends content to a file, optionally creating it if it doesn't exist.

**Parameters**:
- `filePath`: Path to the file
- `content`: Content to append (must support `operator<<`)
- `createIfNonExistent`: If true, creates file if it doesn't exist (default: true)

**Throws**:
- `std::invalid_argument` if file doesn't exist and `createIfNonExistent` is false
- `std::runtime_error` if file creation fails

**Examples**:
```cpp
// Append string
stevensFileLib::appendToFile("log.txt", "Log entry\n");

// Append number
stevensFileLib::appendToFile("numbers.txt", 42);

// Don't create if doesn't exist
stevensFileLib::appendToFile("existing.txt", "data", false);
```

### File Reading

#### `loadFileIntoVector`
```cpp
std::vector<std::string> loadFileIntoVector(
    const std::string& filePath,
    const std::unordered_map<std::string, std::vector<std::string>>& settingsMap = {},
    char separator = '\n',
    bool skipEmptyLines = true)
```
Loads file contents line-by-line into a vector with optional filtering.

**Parameters**:
- `filePath`: Path to the file
- `settingsMap`: Settings for filtering lines
  - `"skip if starts with"`: Vector of prefixes to skip
  - `"skip if contains"`: Vector of substrings to skip
- `separator`: Line separator character (default: `'\n'`)
- `skipEmptyLines`: Skip empty lines (default: true)

**Returns**: Vector of strings containing file lines

**Throws**: `std::invalid_argument` if file cannot be opened

**Examples**:
```cpp
// Simple read
auto lines = stevensFileLib::loadFileIntoVector("data.txt");

// Skip comments
std::unordered_map<std::string, std::vector<std::string>> settings;
settings["skip if starts with"] = {"#", "//"};
auto lines = stevensFileLib::loadFileIntoVector("config.txt", settings);

// Keep empty lines
auto lines = stevensFileLib::loadFileIntoVector("file.txt", {}, '\n', false);

// Custom separator
auto parts = stevensFileLib::loadFileIntoVector("data.csv", {}, ',');

// Multiple filters
settings["skip if starts with"] = {"#", "//"};
settings["skip if contains"] = {"ERROR", "WARNING"};
auto lines = stevensFileLib::loadFileIntoVector("log.txt", settings);
```

#### `loadFileIntoVectorOfInts`
```cpp
std::vector<int> loadFileIntoVectorOfInts(
    const std::string& filePath,
    const std::unordered_map<std::string, std::vector<std::string>>& settingsMap = {},
    char separator = '\n',
    bool skipEmptyLines = true)
```
Loads file contents into a vector of integers.

**Returns**: Vector of integers parsed from file

**Examples**:
```cpp
// Read space-separated integers: "1 2 3 4 5"
auto numbers = stevensFileLib::loadFileIntoVectorOfInts("numbers.txt");

// Read newline-separated integers
auto numbers = stevensFileLib::loadFileIntoVectorOfInts("data.txt");
```

#### `getRandomFileLine`
```cpp
std::string getRandomFileLine(const std::string& filePath, char separator = '\n')
```
Returns a random line from a file using modern C++ random generation.

**Returns**: Random line from the file

**Throws**:
- `std::invalid_argument` if file cannot be opened
- `std::runtime_error` if file is empty

**Examples**:
```cpp
// Get random line
std::string quote = stevensFileLib::getRandomFileLine("quotes.txt");

// Custom separator
std::string item = stevensFileLib::getRandomFileLine("items.csv", ',');
```

### Directory Operations

#### `listFiles`
```cpp
std::vector<std::string> listFiles(
    const std::string& directoryPath,
    const std::unordered_map<std::string, std::string>& settingsMap = {
        {"targetFileExtensions", ""},
        {"excludeFileExtensions", ""},
        {"excludeFiles", ""}
    })
```
Lists all files in a directory with optional filtering.

**Parameters**:
- `directoryPath`: Path to the directory
- `settingsMap`: Settings for filtering files
  - `"targetFileExtensions"`: Comma-separated extensions to include (e.g., ".txt,.cpp")
  - `"excludeFileExtensions"`: Comma-separated extensions to exclude
  - `"excludeFiles"`: Comma-separated filenames to exclude

**Returns**: Vector of filenames (not full paths)

**Throws**: `std::invalid_argument` if directory doesn't exist

**Examples**:
```cpp
// List all files
auto files = stevensFileLib::listFiles("./data");

// Only text files
std::unordered_map<std::string, std::string> settings;
settings["targetFileExtensions"] = ".txt";
auto textFiles = stevensFileLib::listFiles("./data", settings);

// Multiple extensions
settings["targetFileExtensions"] = ".cpp,.hpp,.h";
auto sourceFiles = stevensFileLib::listFiles("./src", settings);

// Exclude extensions
settings["excludeFileExtensions"] = ".tmp,.bak";
auto files = stevensFileLib::listFiles("./", settings);

// Exclude specific files
settings["excludeFiles"] = "README.md,LICENSE";
auto files = stevensFileLib::listFiles("./", settings);

// Combined filters
settings["targetFileExtensions"] = ".txt";
settings["excludeFiles"] = "temp.txt,old.txt";
auto files = stevensFileLib::listFiles("./docs", settings);
```

## Code Quality Features

This library has been refactored with the following best practices:

### Simplicity & Readability
- Clear, descriptive function and variable names
- Well-organized code with logical sections
- Comprehensive documentation and examples

### Functional Abstraction
- Helper functions extracted to `internal` namespace
- Single Responsibility Principle applied
- Structured configuration classes instead of magic strings

### Encapsulation
- `LoadSettings` and `ListFilesSettings` configuration structures
- Internal utilities hidden in `internal` namespace
- Clean public API

### DRY (Don't Repeat Yourself)
- Extracted `shouldSkipLine` and `shouldIncludeFile` helpers
- Eliminated duplicate filtering logic
- Reusable string parsing utilities

### Efficiency & Speed
- Minimal nesting (max 2 levels per function)
- Modern C++ random generation (`std::mt19937` instead of `rand()`)
- Pass-by-reference for large objects
- Optimized file operations

### Good Variable Names
- `lines` instead of `vec`
- `shouldSkipLine` instead of `skip`
- `separator` instead of single-letter variables
- Clear parameter names

### Maximum 2 Levels of Nesting
All functions maintain maximum 2 levels of nesting for readability:
```cpp
// Example from loadFileIntoVector
while (std::getline(file, line, separator))  // Level 1
{
    if (!internal::shouldSkipLine(line, settings))  // Level 2
        lines.push_back(line);
}
```

## Refactoring Summary

### Major Improvements

1. **Removed External Dependency**: Implemented `startsWith`, `contains`, and `splitString` internally
2. **Added Header Guards**: Proper include guards prevent multiple inclusion
3. **Structured Configuration**: Replaced magic strings with `LoadSettings` and `ListFilesSettings`
4. **Reduced Nesting**: All functions now have max 2 levels of nesting
5. **Better Error Messages**: More descriptive exception messages
6. **Modern C++ Random**: Replaced `rand()` with `<random>` library
7. **Improved Documentation**: Comprehensive doxygen comments
8. **Optimizations**: Eliminated unnecessary copies and improved efficiency

### Before vs After

**Before** (loadFileIntoVector):
- 3-4 levels of nesting
- Poor variable names (`vec`, `skip`, `i`)
- Magic strings for settings
- Inefficient array indexing

**After**:
- Max 2 levels of nesting
- Clear names (`lines`, `shouldSkipLine`)
- Structured configuration
- Range-based loops and helper functions

## Testing

The library includes comprehensive tests covering:

- File opening and validation
- File writing and appending
- File reading with various filters
- Integer file parsing
- Random line selection
- Directory listing with filters
- Edge cases and error conditions

Test coverage includes:
- Happy path scenarios
- Error conditions
- Edge cases (empty files, non-existent files)
- Multiple filtering combinations
- Different file formats and separators

## Benchmarks

Performance benchmarks are included for:

- Loading files of various sizes (small, medium, large)
- File loading with filtering
- Integer file parsing
- File appending (small and large content)
- Random line selection
- Directory listing with various filters

Run benchmarks to measure performance on your system:
```bash
./stevensFileLib_benchmarks
```

## Installation

### CMake Integration

```cmake
find_package(stevensFileLib REQUIRED)
target_link_libraries(your_target stevensFileLib::stevensFileLib)
```

### Manual Installation

Copy `stevensFileLib.hpp` to your include directory:
```bash
cp stevensFileLib.hpp /usr/local/include/
```

## License

MIT License - Copyright (c) 2024 Jeff Stevens

## Author

Jeff Stevens (jeff@bucephalusstudios.com)
Bucephalus Studios

## Contributing

Contributions are welcome! Please ensure:
- Code follows the established style
- Max 2 levels of nesting per function
- Comprehensive tests for new features
- Documentation is updated

## Version History

### 0.2 (2024-11-14)
- Complete refactoring for code quality
- Removed stevensStringLib dependency
- Added CMake build system
- Added Google Test unit tests
- Added Google Benchmark performance tests
- Reduced nesting to max 2 levels
- Improved variable names and documentation
- Added structured configuration classes
- Optimized for performance

### 0.1 (2024-02-21)
- Initial release
- Basic file operations
- Directory listing
