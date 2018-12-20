//
// Created by gogagum on 26.10.18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "BinomialHeap.hpp"

using std::string;
using std::ifstream;

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

TEST(BinomialHeapConstructor, DefaultConstructor) {
    BinomialHeap<int> test_heap;
    EXPECT_EQ(test_heap.size(), 0);
}

TEST(BinomialHeapConstructor, ConstructorFromArray) {
    int array[5];
    for (int i = 0; i < 5; i++) {
        array[i] = i;
    }
    BinomialHeap<int> test_heap(array, 5);
    EXPECT_EQ(test_heap.size(), 5);
    EXPECT_EQ(test_heap.get_min(), 0);
}

TEST(BinomialHeapConstructor, copy_constructor) {
    int array[5];
    for (int i = 0; i < 5; i++) {
        array[i] = i;
    }
    BinomialHeap<int> test_heap1(array, 5);
    BinomialHeap<int> test_heap2(test_heap1);
    EXPECT_EQ(test_heap2.size(), 5);
    EXPECT_EQ(test_heap2.get_min(), 0);
}

TEST(BinomialHeapMethods, insert) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinomialHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.size(), 10);
    test_heap.insert(-6);
    EXPECT_EQ(test_heap.get_min(), -6);
}

TEST(BinomialHeapMethods, merge_two_heaps) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    BinomialHeap<int> result_heap = merge(test_heap1, test_heap2);
    EXPECT_EQ(result_heap.size(), 20);
    EXPECT_EQ(result_heap.get_min(), 1);
    EXPECT_EQ(test_heap1.size(), 0);
    EXPECT_EQ(test_heap2.size(), 0);
}

TEST(BinomialHeapMethods, merge_heaps_copies) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    BinomialHeap<int> result_heap = merge_heaps_copies(test_heap1, test_heap2);
    EXPECT_EQ(result_heap.size(), 20);
    EXPECT_EQ(result_heap.get_min(), 1);
    EXPECT_EQ(test_heap1.size(), 10);
    EXPECT_EQ(test_heap2.size(), 10);
}

TEST(BinomialHeapMethods, merge) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    test_heap2.merge(test_heap1);
    EXPECT_EQ(test_heap1.size(), 0);
    EXPECT_EQ(test_heap2.get_min(), 1);
}

TEST(BinomialHeapMethods, extract_min) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinomialHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.size(), 10);
    EXPECT_EQ(test_heap.extract_min(), 1);
    EXPECT_EQ(test_heap.get_min(), 2);
    EXPECT_EQ(test_heap.size(), 9);
}

void big_insert_test(string test_file_name) {
    ifstream test_file(test_file_name);
    int test_size;
    test_file >> test_size;
    BinomialHeap<int> test_heap;

    for (int i = 0; i < test_size; ++i) {
        int insert_key;
        test_file >> insert_key;
        test_heap.insert(insert_key);
        int current_min;
        test_file >> current_min;
        EXPECT_EQ(current_min, test_heap.get_min());
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
    BinomialHeap<int> test_heap;

    for (int i = 0; i < test_size; ++i) {
        int query_type;
        test_file >> query_type;
        int key;
        test_file >> key;
        if (query_type == 0) {
            test_heap.insert(key);
        }
        else {
            EXPECT_EQ(test_heap.extract_min(), key);
        }
    }

    test_file.close();
}

TEST(BigTests, InsertAndExtractRoorTests) {
    for (int i = 1; i <= 20; ++i ) {
        big_extract_root_test("extract_root_tests/test_" + int_to_str(i) + ".txt");
    }
}