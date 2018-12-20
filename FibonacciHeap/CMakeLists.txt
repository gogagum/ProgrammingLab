cmake_minimum_required(VERSION 3.12)
project(FibonacciHeap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

set(SOURCE_FILES main.cpp lib/Tests/FibonacciHeap.hpp lib/Tests/test.cpp)
add_executable(FibonacciHeap ${SOURCE_FILES} lib/Tests/FibonacciHeap.hpp lib/Tests/test.cpp)
target_link_libraries(FibonacciHeap gtest gtest_main)
