cmake_minimum_required(VERSION 3.12)
project(BinaryHeap)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

set(SOURCE_FILES main.cpp lib/Tests/BinaryHeap.hpp lib/Tests/test.cpp)
add_executable(BinaryHeap ${SOURCE_FILES} lib/Tests/BinaryHeap.hpp lib/Tests/test.cpp)
target_link_libraries(BinaryHeap gtest gtest_main)
