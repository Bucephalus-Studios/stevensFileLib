add_executable(tests
    test_file_operations.cpp
    test_directory_operations.cpp
)

target_link_libraries(tests
    PRIVATE
        stevensFileLib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)