cmake_minimum_required(VERSION 3.12)
project(BinomialHeap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

set(SOURCE_FILES main.cpp src/BinomialHeap.hpp lib/Tests/test.cpp)
add_executable(BinomialHeap ${SOURCE_FILES} src/BinomialHeap.hpp lib/Tests/test.cpp)
target_link_libraries(BinomialHeap gtest gtest_main)
