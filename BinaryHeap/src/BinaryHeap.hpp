//
// Created by gogagum on 25.10.18.
//

#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include "DynamicArray.hpp"
#include <glob.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <lzma.h>

template <typename  T>
bool default_comparator(T element1, T element2) {
    return element1 < element2;
}

int parent_index(int index) {
    return (index - 1) / 2;
}

template <typename KeyType>
class node_ptr{
public:
    template <typename> friend class BinaryHeap;
    node_ptr() = default;
    node_ptr(const node_ptr<KeyType>& other) : key_ptr_(other.key_ptr_), index_ptr_(other.index_ptr_) { }
    node_ptr<KeyType>& operator = (const node_ptr<KeyType> & other) {
        key_ptr_ = other.key_ptr_;
        index_ptr_ = other.index_ptr_;
        return *this;
    }
    ~node_ptr() = default;
private:
    node_ptr(int index, const std::shared_ptr<KeyType>& key_ptr) : key_ptr_(key_ptr), index_ptr_(new int) {
        *index_ptr_ = index;
    };
    KeyType& key() {
        return *key_ptr_;
    }
    KeyType key() const {
        return *key_ptr_;
    }
    int& index() {
        return *index_ptr_;
    };
    std::shared_ptr<KeyType> key_ptr_;
    std::shared_ptr<int> index_ptr_;
};

template <typename KeyType>
class BinaryHeap {
public:
    explicit BinaryHeap(bool (*comp)(KeyType, KeyType) = default_comparator) : size_(0), buffer_(0, node_ptr<KeyType>()), comp_(comp) { }

    BinaryHeap(KeyType* array, size_t size, bool (*comp)(KeyType, KeyType) = default_comparator) :
     size_(size), buffer_(size, node_ptr<KeyType>()), comp_(comp) {
        for (int i = 0; i < size_; ++i) {
            buffer_[i].key_ptr_ = std::shared_ptr<KeyType>(new KeyType);
            buffer_[i].index_ptr_ = std::shared_ptr<KeyType>(new int);
            buffer_[i].key() = array[i];
            buffer_[i].index() = i;
        }
        FixHeapProperty();
    }

    template<class Iterator>
    BinaryHeap(Iterator iterator1, Iterator iterator2, bool (*comp)(KeyType, KeyType) = default_comparator) : comp_(comp), size_(){
        int index = 0;
        for (auto i = iterator1; i != iterator2; ++i) {
            std::shared_ptr<KeyType> new_key_ptr(new KeyType (*i));
            node_ptr<KeyType> new_node_ptr(index++, new_key_ptr);
            buffer_.PushBack(new_node_ptr);
            ++size_;
        }
        FixHeapProperty();
    }

    void SiftUp(int index) {
        while (index != 0 && comp_(*(buffer_[index].key_ptr_), *(buffer_[parent_index(index)].key_ptr_)) ) {
            std::swap( buffer_[index], buffer_[parent_index(index)]);
            std::swap( buffer_[index].index(), buffer_[parent_index(index)].index());
            index = parent_index(index);
        }
    }

    void SiftDown(int index) {
        while ( 2 * index + 1 < size_ ) {
            int left = 2 * index + 1;
            int right = 2  * index + 2;
            int index_to_swap = left;
            if (right < size_ && comp_(buffer_[right].key(), buffer_[left].key())) {
                index_to_swap = right;
            }
            if (comp_(buffer_[index].key(), buffer_[index_to_swap].key())) {
                break;
            }
            std::swap(buffer_[index], buffer_[index_to_swap]);
            std::swap(buffer_[index].index(), buffer_[index_to_swap].index());
            index = index_to_swap;
        }
    }

    node_ptr<KeyType> Insert(KeyType key) {
        std::shared_ptr<KeyType> new_key_ptr(new KeyType (key));
        node_ptr<KeyType> new_node_ptr(size_, new_key_ptr);

        buffer_.PushBack(new_node_ptr);
        SiftUp(size_);
        ++size_;
        return new_node_ptr;
    }

    KeyType GetRoot() const {
        assert (!Empty());
        return buffer_[0].key();
    }

    KeyType ExtractRoot() {
        assert (!Empty());
        KeyType to_return = buffer_[0].key();
        buffer_[0] = buffer_[size_-1];
        buffer_.Erase(size_ - 1);
        --size_;
        SiftDown(0);
        return to_return;
    }

    void Erase(node_ptr<KeyType> pointer) {
        int erase_index = *pointer.index_ptr_;
        std::swap(buffer_[size_ - 1], buffer_[erase_index]);
        std::swap(buffer_[size_ - 1].index(), buffer_[erase_index].index());
        buffer_.Erase(size_ - 1);
        --size_;
        if (erase_index > 0 && comp_(buffer_[erase_index].key(), buffer_[parent_index(erase_index)].key())) {
            SiftUp(erase_index);
        }
        else {
            SiftDown(erase_index);
        }
    }

    void Change( node_ptr < KeyType > pointer, KeyType key) {
        int change_index = pointer.index();
        pointer.key() = key;
        if (change_index > 0 && comp_(buffer_[change_index].key(), buffer_[parent_index(change_index)].key())) {
            SiftUp(change_index);
        }
        else {
            SiftDown(change_index);
        }
    }

    bool Empty() const {
        return (size_ == 0);
    }

    size_t Size() const {
        return size_;
    }

    ~ BinaryHeap() = default;
private:
    void FixHeapProperty() {
        for (int i  = size_; i >= 0; --i) {
            SiftDown(i);
        }
    }

    DynamicArray<node_ptr<KeyType>> buffer_;
    size_t size_;
    bool (*comp_)(KeyType, KeyType);
};


#endif //BINARY_HEAP_H
