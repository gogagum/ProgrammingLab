//
// Created by gogagum on 26.10.18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "BinomialHeap.hpp"

using std::string;
using std::ifstream;
using std::to_string;

TEST(BinomialHeapConstructor, DefaultConstructor) {
    BinomialHeap<int> test_heap;
    EXPECT_EQ(test_heap.Size(), 0);
}

TEST(BinomialHeapConstructor, ConstructorFromArray) {
    int array[5];
    for (int i = 0; i < 5; i++) {
        array[i] = i;
    }
    BinomialHeap<int> test_heap(array, 5);
    EXPECT_EQ(test_heap.Size(), 5);
    EXPECT_EQ(test_heap.GetMin(), 0);
}

TEST(BinomialHeapConstructor, copy_constructor) {
    int array[5];
    for (int i = 0; i < 5; i++) {
        array[i] = i;
    }
    BinomialHeap<int> test_heap1(array, 5);
    BinomialHeap<int> test_heap2(test_heap1);
    EXPECT_EQ(test_heap2.Size(), 5);
    EXPECT_EQ(test_heap2.GetMin(), 0);
}

TEST(BinomialHeapMethods, insert) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinomialHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.Size(), 10);
    test_heap.Insert(-6);
    EXPECT_EQ(test_heap.GetMin(), -6);
}

TEST(BinomialHeapMethods, merge_two_heaps) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    BinomialHeap<int> result_heap = Merge(test_heap1, test_heap2);
    EXPECT_EQ(result_heap.Size(), 20);
    EXPECT_EQ(result_heap.GetMin(), 1);
    EXPECT_EQ(test_heap1.Size(), 0);
    EXPECT_EQ(test_heap2.Size(), 0);
}

TEST(BinomialHeapMethods, MergeHeapsCopies) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    BinomialHeap<int> result_heap = MergeHeapsCopies(test_heap1, test_heap2);
    EXPECT_EQ(result_heap.Size(), 20);
    EXPECT_EQ(result_heap.GetMin(), 1);
    EXPECT_EQ(test_heap1.Size(), 10);
    EXPECT_EQ(test_heap2.Size(), 10);
}

TEST(BinomialHeapMethods, Merge) {
    int array1[10] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int array2[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    BinomialHeap<int> test_heap1(array1, 10);
    BinomialHeap<int> test_heap2(array2, 10);
    test_heap2.Merge(test_heap1);
    EXPECT_EQ(test_heap1.Size(), 0);
    EXPECT_EQ(test_heap2.GetMin(), 1);
}

TEST(BinomialHeapMethods, extract_min) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinomialHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.Size(), 10);
    EXPECT_EQ(test_heap.ExtractMin(), 1);
    EXPECT_EQ(test_heap.GetMin(), 2);
    EXPECT_EQ(test_heap.Size(), 9);
}

void big_insert_test(string test_file_name) {
    ifstream test_file(test_file_name);
    int test_size;
    test_file >> test_size;
    BinomialHeap<int> test_heap;

    for (int i = 0; i < test_size; ++i) {
        int insert_key;
        test_file >> insert_key;
        test_heap.Insert(insert_key);
        int current_min;
        test_file >> current_min;
        EXPECT_EQ(current_min, test_heap.GetMin());
    }

    test_file.close();
}

TEST(BigTests, InsertTests) {
    for (int i = 1; i <= 20; ++i ) {
        big_insert_test("insert_tests/test_" + to_string(i) + ".txt");
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
            test_heap.Insert(key);
        }
        else {
            EXPECT_EQ(test_heap.ExtractMin(), key);
        }
    }

    test_file.close();
}

TEST(BigTests, InsertAndExtractRoorTests) {
    for (int i = 1; i <= 20; ++i ) {
        big_extract_root_test("extract_root_tests/test_" + to_string(i) + ".txt");
    }
}