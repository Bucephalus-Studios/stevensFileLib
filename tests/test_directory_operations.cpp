#include "stevensFileLib.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class DirectoryOperationsTest : public ::testing::Test
{
protected:
    const std::string testDir = "test_directory";

    void SetUp() override
    {
        fs::create_directories(testDir);
    }

    void TearDown() override
    {
        if (fs::exists(testDir))
            fs::remove_all(testDir);
    }

    void createFile(const std::string& filename)
    {
        std::ofstream file(testDir + "/" + filename);
        file << "test content";
    }
};

// ============================================================================
// Tests for listFiles
// ============================================================================

TEST_F(DirectoryOperationsTest, ListFiles_EmptyDirectory_ReturnsEmptyVector)
{
    auto files = stevensFileLib::listFiles(testDir);

    EXPECT_TRUE(files.empty());
}

TEST_F(DirectoryOperationsTest, ListFiles_MultipleFiles_ReturnsAllFiles)
{
    createFile("file1.txt");
    createFile("file2.txt");
    createFile("file3.txt");

    auto files = stevensFileLib::listFiles(testDir);

    ASSERT_EQ(files.size(), 3);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.txt") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_DirectoryDoesNotExist_ThrowsException)
{
    EXPECT_THROW(stevensFileLib::listFiles("nonexistent_directory"),
                 std::invalid_argument);
}

TEST_F(DirectoryOperationsTest, ListFiles_TargetExtensions_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.txt");
    createFile("file4.hpp");

    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.txt") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_MultipleTargetExtensions_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.hpp");
    createFile("file4.md");

    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".cpp,.hpp";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.cpp") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.hpp") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_ExcludeExtensions_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.txt");
    createFile("file4.hpp");

    std::unordered_map<std::string, std::string> settings;
    settings["excludeFileExtensions"] = ".txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.cpp") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file4.hpp") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_MultipleExcludeExtensions_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.hpp");
    createFile("file4.md");

    std::unordered_map<std::string, std::string> settings;
    settings["excludeFileExtensions"] = ".txt,.md";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.cpp") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.hpp") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_ExcludeSpecificFiles_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.txt");
    createFile("file3.txt");
    createFile("excluded.txt");

    std::unordered_map<std::string, std::string> settings;
    settings["excludeFiles"] = "excluded.txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 3);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "excluded.txt") == files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_MultipleExcludeFiles_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.txt");
    createFile("excluded1.txt");
    createFile("excluded2.txt");

    std::unordered_map<std::string, std::string> settings;
    settings["excludeFiles"] = "excluded1.txt,excluded2.txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.txt") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_CombinedFilters_FiltersCorrectly)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.txt");
    createFile("excluded.txt");
    createFile("file4.hpp");

    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".txt";
    settings["excludeFiles"] = "excluded.txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file3.txt") != files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_EmptySettings_ReturnsAllFiles)
{
    createFile("file1.txt");
    createFile("file2.cpp");
    createFile("file3.hpp");

    std::unordered_map<std::string, std::string> settings;

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 3);
}

TEST_F(DirectoryOperationsTest, ListFiles_WithSubdirectory_OnlyListsFilesNotDirectories)
{
    createFile("file1.txt");
    fs::create_directory(testDir + "/subdir");
    createFile("file2.txt");

    auto files = stevensFileLib::listFiles(testDir);

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file1.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "file2.txt") != files.end());
    EXPECT_TRUE(std::find(files.begin(), files.end(), "subdir") == files.end());
}

TEST_F(DirectoryOperationsTest, ListFiles_MixedExtensionsNoFilter_ReturnsAll)
{
    createFile("document.txt");
    createFile("image.png");
    createFile("script.py");
    createFile("source.cpp");

    auto files = stevensFileLib::listFiles(testDir);

    EXPECT_EQ(files.size(), 4);
}

TEST_F(DirectoryOperationsTest, ListFiles_FileWithNoExtension_HandledCorrectly)
{
    createFile("README");
    createFile("LICENSE");
    createFile("file.txt");

    std::unordered_map<std::string, std::string> settings;
    settings["targetFileExtensions"] = ".txt";

    auto files = stevensFileLib::listFiles(testDir, settings);

    ASSERT_EQ(files.size(), 1);
    EXPECT_EQ(files[0], "file.txt");
}
