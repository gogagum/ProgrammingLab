//
// Created by gogagum on 25.10.18.
//

#ifndef HEAP_HPP
#define HEAP_HPP

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

template <typename KeyType>
class node_ptr{
public:
    template <typename> friend class Heap;
    node_ptr() { };
    node_ptr(const node_ptr<KeyType> & other) : key_ptr_(other.key_ptr_), index_ptr_(other.index_ptr_) { }
    void operator = (const node_ptr<KeyType> & other) {
        key_ptr_ = other.key_ptr_;
        index_ptr_ = other.index_ptr_;
    }
    ~node_ptr() { }
private:
    node_ptr(int index, const std::shared_ptr<KeyType> & key_ptr) : key_ptr_(key_ptr), index_ptr_(new int) {
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
class Heap {
public:
    Heap(bool (*comp)(KeyType, KeyType) = default_comparator) : size_(0), buffer_(0, node_ptr<KeyType>()), comp_(comp) { }

    Heap(KeyType* array, size_t size, bool (*comp)(KeyType, KeyType) = default_comparator) :
     size_(size), buffer_(size, node_ptr<KeyType>()), comp_(comp) {
        for (int i = 0; i < size_; ++i) {
            buffer_[i].key_ptr_ = std::shared_ptr<KeyType>(new KeyType);
            buffer_[i].index_ptr_ = std::shared_ptr<KeyType>(new int);
            buffer_[i].key() = array[i];
            buffer_[i].index() = i;
        }
        heapify();
    }

    template<class Iterator>
    Heap(Iterator iterator1, Iterator iterator2, bool (*comp)(KeyType, KeyType) = default_comparator) : comp_(comp), size_(){
        int index = 0;
        for (auto i = iterator1; i != iterator2; ++i) {
            std::shared_ptr<KeyType> new_key_ptr(new KeyType (*i));
            node_ptr<KeyType> new_node_ptr(index++, new_key_ptr);
            buffer_.push_back(new_node_ptr);
            ++size_;
        }
        heapify();
    }

    void sift_up (int index) {
        while ( index != 0 && comp_(*(buffer_[index].key_ptr_), *(buffer_[(index - 1) / 2].key_ptr_)) ) {
            std::swap( buffer_[index], buffer_[(index - 1) / 2]);
            std::swap( buffer_[index].index(), buffer_[(index - 1) / 2].index());
            index = (index - 1) / 2;
        }
    }

    void sift_down (int index) {
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

    node_ptr<KeyType> insert(KeyType key) {
        std::shared_ptr<KeyType> new_key_ptr(new KeyType (key));
        node_ptr<KeyType> new_node_ptr(size_, new_key_ptr);

        buffer_.push_back(new_node_ptr);
        size_++;
        sift_up(size_ - 1);
        return new_node_ptr;
    }

    KeyType get_root() const {
        assert (!empty());
        return buffer_[0].key();
    }

    KeyType extract_root() {
        assert (!empty());
        KeyType to_return = buffer_[0].key();
        buffer_[0] = buffer_[size_-1];
        buffer_.erase(size_ - 1);
        size_--;
        sift_down(0);
        return to_return;
    }

    void erase(node_ptr<KeyType> pointer) {
        int erase_index = *pointer.index_ptr_;
        std::swap(buffer_[size_ - 1], buffer_[erase_index]);
        std::swap(buffer_[size_ - 1].index(), buffer_[erase_index].index());
        buffer_.erase(size_ - 1);
        size_ --;
        if (erase_index > 0 && comp_(buffer_[erase_index].key(), buffer_[(erase_index - 1) / 2].key())) {
            sift_up(erase_index);
        }
        else {
            sift_down(erase_index);
        }
    }

    void change( node_ptr < KeyType > pointer, KeyType key) {
        int change_index = pointer.index();
        pointer.key() = key;
        if (change_index > 0 && comp_(buffer_[change_index].key(), buffer_[(change_index - 1) / 2].key())) {
            sift_up(change_index);
        }
        else {
            sift_down(change_index);
        }
    }

    bool empty() const {
        return (size_ == 0);
    }

    size_t size() const {
        return size_;
    }

    ~ Heap() { }
private:
    void heapify() {
        for (int i  = size_; i >= 0; --i) {
            sift_down(i);
        }
    }

    DynamicArray< node_ptr<KeyType> > buffer_;
    size_t size_;
    bool (*comp_)(KeyType, KeyType);
};


#endif //HEAP_HEAP_H
