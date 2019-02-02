//
// Created by gogagum on 25.11.18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <string>
#include "/home/gogagum/CLionProjects/Lab/FibonacciHeap/src/FibonacciHeap.hpp"

using std::ifstream;
using std::string;

string int_to_str(unsigned int num){
    string ret = "";
    if (num == 0) {
        return "0";
    }
    while (num != 0) {
        ret = (char)('0' + num % 10) + ret;
        num /= 10;
    }
    return ret;
}

TEST (FibonacciHeapConstructor, default_constructor) {
    FibonacciHeap<int> test_heap;
    EXPECT_EQ(test_heap.Size(), 0);
}

TEST (FibonacciHeapMethods, insert) {
    FibonacciHeap<int> test_heap;
    test_heap.Insert(5);
    EXPECT_EQ(test_heap.GetRoot(), 5);
    EXPECT_EQ(test_heap.Size(), 1);
}

TEST (FibonacciHeapMethods, insert2) {
    FibonacciHeap<int> test_heap;
    test_heap.Insert(5);
    EXPECT_EQ(test_heap.GetRoot(), 5);
    test_heap.Insert(3);
    EXPECT_EQ(test_heap.GetRoot(), 3);
    test_heap.Insert(6);
    EXPECT_EQ(test_heap.GetRoot(), 3);
    EXPECT_EQ(test_heap.Size(), 3);
}

TEST (FibonacciHeapMethods, merge) {
    FibonacciHeap<int> first;
    FibonacciHeap<int> second;
    for (int i = 0; i < 5; i++) {
        first.Insert(2 * i + 1);
        second.Insert(2 * i);
    }
    first.Merge(second);
    EXPECT_EQ(second.Empty(), true);
    EXPECT_EQ(first.GetRoot(), 0);
    EXPECT_EQ(first.Size(), 10);
}

TEST (FibonacciHeapMethods, merge2) {
    FibonacciHeap<int> first;
    FibonacciHeap<int> second;
    FibonacciHeap<int> third;
    for (int i = 0; i < 5; i++) {
        first.Insert(3 * i);
        second.Insert(3 * i + 1);
        third.Insert(3 * i + 2);
    }
    second.Merge(first);
    third.Merge(second);
    EXPECT_EQ(first.Empty(), true);
    EXPECT_EQ(second.Empty(), true);
    EXPECT_EQ(third.Empty(), false);
    EXPECT_EQ(third.GetRoot(), 0);
    EXPECT_EQ(third.Size(), 15);
}

TEST (FibonacciHeapMethods, extract_root) {
    FibonacciHeap<int> test_heap;
    for (int i = 0; i < 5; i++) {
        test_heap.Insert(i);
    }
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(test_heap.ExtractRoot(), i);
        EXPECT_EQ(test_heap.Size(), 5 - i - 1);
    }
    EXPECT_EQ(test_heap.Size(), 0);
}

TEST (FibonacciHeapMethods, decrease_key) {
    FibonacciHeap<int> test_heap;
    auto to_dectease = test_heap.Insert(5);
    test_heap.DecreaseKey(to_dectease, 4);
    EXPECT_EQ(test_heap.GetRoot(), 4);
}


TEST (FibonacciHeapMethods, decrease_key2) {
    FibonacciHeap<int> test_heap;
    auto to_dectease = test_heap.Insert(5);
    test_heap.Insert(3);
    test_heap.DecreaseKey(to_dectease, 2);
    EXPECT_EQ(test_heap.ExtractRoot(), 2);
    EXPECT_EQ(test_heap.GetRoot(), 3);
}

void big_insert_test(string test_file_name) {
    ifstream test_file(test_file_name);
    int test_size;
    test_file >> test_size;
    FibonacciHeap<int> test_heap;

    for (int i = 0; i < test_size; ++i) {
        int insert_key;
        test_file >> insert_key;
        test_heap.Insert(insert_key);
        int current_min;
        test_file >> current_min;
        EXPECT_EQ(current_min, test_heap.GetRoot());
    }

    test_file.close();
}

TEST(BigTests, InsertTests) {
    for (int i = 1; i <= 20; ++i ) {
        big_insert_test("insert_tests/test_" + int_to_str(i) + ".txt");
    }
}

void big_extract_root_test(string test_file_name) {
    ifstream test_file(test_file_name);
    int test_size;
    test_file >> test_size;
    FibonacciHeap<int> test_heap;

    for (int i = 0; i < test_size; ++i) {
        int query_type;
        test_file >> query_type;
        int key;
        test_file >> key;
        if (query_type == 0) {
            test_heap.Insert(key);
        }
        else {
            EXPECT_EQ(test_heap.ExtractRoot(), key);
        }
    }

    test_file.close();
}

TEST(BigTests, InsertAndExtractRootTests) {
    for (int i = 1; i <= 20; ++i ) {
        big_extract_root_test("extract_root_tests/test_" + int_to_str(i) + ".txt");
    }
}