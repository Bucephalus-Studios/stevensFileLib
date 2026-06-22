cmake_minimum_required(VERSION 3.16)
project(stevensFileLib LANGUAGES CXX)

add_library(stevensFileLib INTERFACE)
add_library(stevensFileLib::stevensFileLib ALIAS stevensFileLib)

target_include_directories(stevensFileLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(stevensFileLib INTERFACE cxx_std_17)
