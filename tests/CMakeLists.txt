add_executable(stevensFileLib_tests
    test_file_operations.cpp
    test_directory_operations.cpp
)

target_link_libraries(stevensFileLib_tests
    PRIVATE
        stevensFileLib
        GTest::gtest
        GTest::gtest_main
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(stevensFileLib_tests)
