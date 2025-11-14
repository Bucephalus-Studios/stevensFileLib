#include "stevensFileLib.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileOperationsTest : public ::testing::Test
{
protected:
    const std::string testDir = "test_files";
    const std::string testFile = testDir + "/test.txt";
    const std::string testFileInts = testDir + "/test_ints.txt";

    void SetUp() override
    {
        fs::create_directories(testDir);
    }

    void TearDown() override
    {
        if (fs::exists(testDir))
            fs::remove_all(testDir);
    }

    void createTestFile(const std::string& path, const std::string& content)
    {
        std::ofstream file(path);
        file << content;
    }
};

// ============================================================================
// Tests for openInputFile
// ============================================================================

TEST_F(FileOperationsTest, OpenInputFile_FileExists_OpensSuccessfully)
{
    createTestFile(testFile, "test content");

    auto file = stevensFileLib::openInputFile(testFile);
    EXPECT_TRUE(file.is_open());
}

TEST_F(FileOperationsTest, OpenInputFile_FileDoesNotExist_ThrowsException)
{
    EXPECT_THROW(stevensFileLib::openInputFile("nonexistent.txt"), std::invalid_argument);
}

// ============================================================================
// Tests for openOutputFile
// ============================================================================

TEST_F(FileOperationsTest, OpenOutputFile_FileExists_OpensSuccessfully)
{
    createTestFile(testFile, "");

    auto file = stevensFileLib::openOutputFile(testFile);
    EXPECT_TRUE(file.is_open());
}

TEST_F(FileOperationsTest, OpenOutputFile_FileDoesNotExist_CreatesFile)
{
    std::string newFile = testDir + "/new_file.txt";
    auto file = stevensFileLib::openOutputFile(newFile);
    EXPECT_TRUE(file.is_open());
    file.close();
    EXPECT_TRUE(fs::exists(newFile));
}

// ============================================================================
// Tests for appendToFile
// ============================================================================

TEST_F(FileOperationsTest, AppendToFile_FileExists_AppendsContent)
{
    createTestFile(testFile, "initial content\n");

    stevensFileLib::appendToFile(testFile, "appended content\n");

    auto lines = stevensFileLib::loadFileIntoVector(testFile, {}, '\n', false);
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "initial content");
    EXPECT_EQ(lines[1], "appended content");
}

TEST_F(FileOperationsTest, AppendToFile_FileDoesNotExist_CreatesAndWrites)
{
    std::string newFile = testDir + "/new_file.txt";

    stevensFileLib::appendToFile(newFile, "new content");

    EXPECT_TRUE(fs::exists(newFile));
    auto lines = stevensFileLib::loadFileIntoVector(newFile);
    ASSERT_EQ(lines.size(), 1);
    EXPECT_EQ(lines[0], "new content");
}

TEST_F(FileOperationsTest, AppendToFile_FileDoesNotExistNoCreate_ThrowsException)
{
    std::string newFile = testDir + "/new_file.txt";

    EXPECT_THROW(stevensFileLib::appendToFile(newFile, "content", false),
                 std::invalid_argument);
}

TEST_F(FileOperationsTest, AppendToFile_MultipleAppends_AllContentPresent)
{
    createTestFile(testFile, "line1\n");

    stevensFileLib::appendToFile(testFile, "line2\n");
    stevensFileLib::appendToFile(testFile, "line3\n");

    auto lines = stevensFileLib::loadFileIntoVector(testFile, {}, '\n', false);
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

// ============================================================================
// Tests for loadFileIntoVector
// ============================================================================

TEST_F(FileOperationsTest, LoadFileIntoVector_SimpleFile_LoadsAllLines)
{
    createTestFile(testFile, "line1\nline2\nline3\n");

    auto lines = stevensFileLib::loadFileIntoVector(testFile);

    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_EmptyLines_SkipsEmptyByDefault)
{
    createTestFile(testFile, "line1\n\nline2\n\nline3\n");

    auto lines = stevensFileLib::loadFileIntoVector(testFile);

    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "line2");
    EXPECT_EQ(lines[2], "line3");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_EmptyLines_KeepsEmptyWhenDisabled)
{
    createTestFile(testFile, "line1\n\nline2\n");

    auto lines = stevensFileLib::loadFileIntoVector(testFile, {}, '\n', false);

    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "line1");
    EXPECT_EQ(lines[1], "");
    EXPECT_EQ(lines[2], "line2");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_SkipIfStartsWith_FiltersCorrectly)
{
    createTestFile(testFile, "# comment\ndata1\n# another comment\ndata2\n");

    std::unordered_map<std::string, std::vector<std::string>> settings;
    settings["skip if starts with"] = {"#"};

    auto lines = stevensFileLib::loadFileIntoVector(testFile, settings);

    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "data1");
    EXPECT_EQ(lines[1], "data2");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_SkipIfContains_FiltersCorrectly)
{
    createTestFile(testFile, "good line\nbad line with ERROR\nanother good line\n");

    std::unordered_map<std::string, std::vector<std::string>> settings;
    settings["skip if contains"] = {"ERROR"};

    auto lines = stevensFileLib::loadFileIntoVector(testFile, settings);

    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "good line");
    EXPECT_EQ(lines[1], "another good line");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_MultipleFilters_AppliesAllFilters)
{
    createTestFile(testFile, "# comment\nvalid data\ndata with ERROR\n// comment\nmore valid data\n");

    std::unordered_map<std::string, std::vector<std::string>> settings;
    settings["skip if starts with"] = {"#", "//"};
    settings["skip if contains"] = {"ERROR"};

    auto lines = stevensFileLib::loadFileIntoVector(testFile, settings);

    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "valid data");
    EXPECT_EQ(lines[1], "more valid data");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_CustomSeparator_SplitsCorrectly)
{
    createTestFile(testFile, "part1|part2|part3");

    auto lines = stevensFileLib::loadFileIntoVector(testFile, {}, '|', false);

    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "part1");
    EXPECT_EQ(lines[1], "part2");
    EXPECT_EQ(lines[2], "part3");
}

TEST_F(FileOperationsTest, LoadFileIntoVector_FileDoesNotExist_ThrowsException)
{
    EXPECT_THROW(stevensFileLib::loadFileIntoVector("nonexistent.txt"),
                 std::invalid_argument);
}

// ============================================================================
// Tests for loadFileIntoVectorOfInts
// ============================================================================

TEST_F(FileOperationsTest, LoadFileIntoVectorOfInts_SimpleIntegers_LoadsCorrectly)
{
    createTestFile(testFileInts, "1 2 3 4 5");

    auto numbers = stevensFileLib::loadFileIntoVectorOfInts(testFileInts);

    ASSERT_EQ(numbers.size(), 5);
    EXPECT_EQ(numbers[0], 1);
    EXPECT_EQ(numbers[1], 2);
    EXPECT_EQ(numbers[2], 3);
    EXPECT_EQ(numbers[3], 4);
    EXPECT_EQ(numbers[4], 5);
}

TEST_F(FileOperationsTest, LoadFileIntoVectorOfInts_NewlineSeparated_LoadsCorrectly)
{
    createTestFile(testFileInts, "10\n20\n30\n");

    auto numbers = stevensFileLib::loadFileIntoVectorOfInts(testFileInts);

    ASSERT_EQ(numbers.size(), 3);
    EXPECT_EQ(numbers[0], 10);
    EXPECT_EQ(numbers[1], 20);
    EXPECT_EQ(numbers[2], 30);
}

TEST_F(FileOperationsTest, LoadFileIntoVectorOfInts_NegativeNumbers_LoadsCorrectly)
{
    createTestFile(testFileInts, "-5 -10 15 -20");

    auto numbers = stevensFileLib::loadFileIntoVectorOfInts(testFileInts);

    ASSERT_EQ(numbers.size(), 4);
    EXPECT_EQ(numbers[0], -5);
    EXPECT_EQ(numbers[1], -10);
    EXPECT_EQ(numbers[2], 15);
    EXPECT_EQ(numbers[3], -20);
}

TEST_F(FileOperationsTest, LoadFileIntoVectorOfInts_FileDoesNotExist_ThrowsException)
{
    EXPECT_THROW(stevensFileLib::loadFileIntoVectorOfInts("nonexistent.txt"),
                 std::invalid_argument);
}

// ============================================================================
// Tests for getRandomFileLine
// ============================================================================

TEST_F(FileOperationsTest, GetRandomFileLine_SingleLine_ReturnsThatLine)
{
    createTestFile(testFile, "only line\n");

    std::string line = stevensFileLib::getRandomFileLine(testFile);

    EXPECT_EQ(line, "only line");
}

TEST_F(FileOperationsTest, GetRandomFileLine_MultipleLines_ReturnsOneLine)
{
    createTestFile(testFile, "line1\nline2\nline3\n");

    std::string line = stevensFileLib::getRandomFileLine(testFile);

    EXPECT_TRUE(line == "line1" || line == "line2" || line == "line3");
}

TEST_F(FileOperationsTest, GetRandomFileLine_EmptyFile_ThrowsException)
{
    createTestFile(testFile, "");

    EXPECT_THROW(stevensFileLib::getRandomFileLine(testFile),
                 std::runtime_error);
}

TEST_F(FileOperationsTest, GetRandomFileLine_MultipleCallsProduceDifferentResults)
{
    createTestFile(testFile, "");
    for (int i = 0; i < 100; ++i)
    {
        std::ofstream file(testFile, std::ios::app);
        file << "line" << i << "\n";
    }

    std::unordered_set<std::string> uniqueLines;
    for (int i = 0; i < 50; ++i)
    {
        uniqueLines.insert(stevensFileLib::getRandomFileLine(testFile));
    }

    // With 100 lines and 50 random selections, we should get at least 10 unique lines
    EXPECT_GE(uniqueLines.size(), 10);
}

TEST_F(FileOperationsTest, GetRandomFileLine_FileDoesNotExist_ThrowsException)
{
    EXPECT_THROW(stevensFileLib::getRandomFileLine("nonexistent.txt"),
                 std::invalid_argument);
}
