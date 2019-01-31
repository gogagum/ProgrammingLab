//
// Created by gogagum on 25.10.18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include "/home/gogagum/CLionProjects/Lab/BinaryHeap/src/BinaryHeap.hpp"

struct complex_num{
    complex_num() {}
    complex_num(double imz, double rmz){
        imz_ = imz;
        rmz_ = rmz;
    }
    double get_module(){
        return sqrt(imz_ * imz_ + rmz_ * rmz_);
    }
    ~complex_num() {}
    double imz_;
    double rmz_;
};

bool operator == (complex_num c1, complex_num c2) {
    return (c1.imz_ == c2.imz_) && (c1.rmz_ == c2.rmz_);
}

bool comp(complex_num n1, complex_num n2) {
    return n1.get_module() < n2.get_module();
}

TEST(HeapConstructor, DefaultConstructor) {
    BinaryHeap<int> test_heap;
    EXPECT_EQ(test_heap.Size(), 0);
}

TEST(HeapConstructor, ConstructorFromArray)
{
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinaryHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.Size(), 10);
    EXPECT_EQ(test_heap.GetRoot(), 1);
    EXPECT_EQ(test_heap.ExtractRoot(), 1);
    EXPECT_EQ(test_heap.ExtractRoot(), 2);
}

TEST(HeapConstructor, ConstructorFromIterators) {
    std::vector<int> test_vector = {5, 4, 3, 2, 1};
    BinaryHeap<int> test_heap (test_vector.begin(), test_vector.end());
    for (int i = 0; i < 5; i++){
        EXPECT_EQ(test_heap.ExtractRoot(), i + 1);
    }
}

TEST(HeapMethods, Insert) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BinaryHeap<int> test_heap(array, 10);
    EXPECT_EQ(test_heap.Size(), 10);
    test_heap.insert(-6);
    EXPECT_EQ(test_heap.GetRoot(), -6);
}

TEST(HeapMethods, ExtractRoot) {
    int array[4] = {1, 2, 3, 4};
    BinaryHeap<int> test_heap(array, 4);
    EXPECT_EQ(test_heap.ExtractRoot(), 1);
    EXPECT_EQ(test_heap.GetRoot(), 2);
}


TEST(HeapConstructor, CustomComparator) {
    BinaryHeap<complex_num> test_heap(comp);
    complex_num c1(1, 2);
    complex_num c2(42, 42);
    complex_num c3(-1, 0);
    test_heap.insert(c1);
    test_heap.insert(c2);
    test_heap.insert(c3);
    EXPECT_EQ(test_heap.GetRoot(), c3);
}

TEST(node_ptr_using, insert_and_erase) {
    int array[4] = {1, 2, 3, 4};
    BinaryHeap<int> test_heap(array, 4);
    EXPECT_EQ(test_heap.GetRoot(), 1);
    auto ptr_m100 = test_heap.insert(-100);
    EXPECT_EQ(test_heap.GetRoot(), -100);
    test_heap.Erase(ptr_m100);
    EXPECT_EQ(test_heap.GetRoot(), 1);
}

TEST(node_ptr_using, insert_and_change) {
    int array[4] = {1, 2, 3, 4};
    BinaryHeap<int> test_heap(array, 4);
    EXPECT_EQ(test_heap.GetRoot(), 1);
    auto ptr_m100 = test_heap.insert(-100);
    EXPECT_EQ(test_heap.GetRoot(), -100);
    test_heap.Change(ptr_m100, -50);
    EXPECT_EQ(test_heap.GetRoot(), -50);
}

TEST(BigTests, test10) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 10; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}

TEST(BigTests, test100) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 100; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}

TEST(BigTests, test1000) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 1000; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}

TEST(BigTests, test10000) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 10000; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}

TEST(BigTests, test100000) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 100000; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}

TEST(BigTests, test1000000) {
    BinaryHeap<int> test_heap;
    int min = rand();
    test_heap.insert(min);
    for (int i = 1; i < 1000000; i++) {
        int new_num = rand();
        test_heap.insert(new_num);
        if (new_num < min) {
            min = new_num;
        }
    };
}
