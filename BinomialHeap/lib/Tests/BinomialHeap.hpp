//
// Created by gogagum on 26.10.18.
//

#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <glob.h>
#include <cassert>
#include <memory>
#include <algorithm>
#include "DynamicArray.hpp"

using std::shared_ptr;
using std::swap;
using std::max;
using std::min;

template <typename  T>
bool default_comparator(T value1, T value2) {
    return value1 < value2;
}

template <typename KeyType>
class BinomialHeap {
public:
    explicit BinomialHeap(bool (*comp)(KeyType, KeyType) = default_comparator)
     : size_(0), roots_(), comp_(comp) { }

    BinomialHeap(KeyType* array, size_t size, bool (*comp)(KeyType, KeyType) = default_comparator)
     : size_(size), comp_(comp) {
        int pop_index = 0;
        for (int i = 0; (size >> i) > 0; ++i) {
            if ((size >> i) & 1) {
                int length_of_block = (1<<i);
                auto nodes_to_merge = new shared_ptr<node<KeyType>>[length_of_block];
                for (int j = 0; j < length_of_block; ++j) {
                    nodes_to_merge[j] = shared_ptr<node<KeyType>>(new node<KeyType>);
                    nodes_to_merge[j]->value_ = array[pop_index];
                    ++pop_index;
                }
                for (int curr_number_of_trees = length_of_block; curr_number_of_trees >= 2; curr_number_of_trees >>= 1) {
                    for (int k = 0; k < curr_number_of_trees / 2; ++k) {
                        nodes_to_merge[k] = merge_trees(nodes_to_merge[k*2], nodes_to_merge[k*2 + 1]);
                    }
                }
                roots_.PushBack(nodes_to_merge[0]);
                delete [] nodes_to_merge;
            }
            else {
                roots_.PushBack(nullptr);
            }
        }
    }

    BinomialHeap(const BinomialHeap<KeyType> &other)
     : size_(other.size_), roots_(), comp_(other.comp_) {
        for (int i = 0; i < other.roots_.Size(); ++i) {
            if (other.roots_[i] != nullptr) {
                shared_ptr<node<KeyType>> copied_node(new node<KeyType>);
                copied_node->copy(*(other.roots_[i]));
                roots_.PushBack(copied_node);
            }
            else {
                roots_.PushBack(nullptr);
            }
        }
    }

    void Insert(KeyType key) {
        BinomialHeap<KeyType> to_merge(comp_);
        shared_ptr<node<KeyType>> one_element_node(new node<KeyType>);
        one_element_node->value_ = key;
        one_element_node->degree_ = 0;
        ++to_merge.size_;
        to_merge.roots_.PushBack(one_element_node);
        Merge(to_merge);
    }

    KeyType GetMin() const {
        assert (!Empty());
        return roots_[find_index_of_min()]->value_;
    }

    KeyType ExtractMin() {
        assert(!Empty());
        auto min_index = find_index_of_min();
        auto min_ptr = roots_[min_index];
        BinomialHeap<KeyType> heap_from_children;
        for (int i = 0; i < min_ptr->children_.Size(); ++i) {
            heap_from_children.roots_.PushBack(min_ptr->children_[i]);
        }
        KeyType to_return = min_ptr->value_;
        roots_[min_index] = nullptr;
        Merge(heap_from_children);
        --size_;
        return to_return;
    }

    void Merge(BinomialHeap<KeyType> &other) { //clears other
        assert(comp_ == other.comp_);
        shared_ptr<node<KeyType>> remembered = nullptr;
        size_ += other.Size();
        auto max_size_of_new_tree = max(roots_.Size(), other.roots_.Size()) + 1;

        for (int i = 0; i < max_size_of_new_tree; ++i) {
            if (i < min(roots_.Size(), other.roots_.Size())) {
                if (roots_[i] != nullptr && other.roots_[i] != nullptr) {
                    auto old_remembered = remembered;
                    remembered = merge_trees(roots_[i], other.roots_[i]);
                    roots_[i] = old_remembered;
                    continue;
                }
                if (roots_[i] != nullptr) { // && other.roots[i] == nullptr
                    if (remembered != nullptr) {
                        remembered = merge_trees(remembered, roots_[i]);
                        roots_[i] = nullptr;
                    }
                    //else { roots[i] = roots[i]; }
                    continue;
                }
                if (other.roots_[i] != nullptr) { // && roots[i] == nullptr
                    if (remembered != nullptr) {
                        remembered = merge_trees(remembered, other.roots_[i]);
                    }
                    else {
                        roots_[i] = other.roots_[i];
                    }
                    continue;
                }
                // other.roots[i] == nullptr && roots[i] == nullptr
                if (remembered != nullptr) {
                    roots_[i] = remembered;
                    remembered = nullptr;
                }
            }
            else {
                if (i < roots_.Size()) {
                    if (remembered != nullptr ) {
                        if (roots_[i] != nullptr) {
                            remembered = merge_trees(remembered, roots_[i]);
                            roots_[i] = nullptr;
                        }
                        else {
                            roots_[i] = remembered;
                            remembered = nullptr;
                            break;
                        }
                    }
                    continue;
                }
                if (i < other.roots_.Size()) {
                    if (remembered == nullptr) {
                        roots_.PushBack(other.roots_[i]);
                    }
                    else {
                        if (other.roots_[i] != nullptr) {
                            remembered = merge_trees(remembered, other.roots_[i]);
                            roots_.PushBack(nullptr);
                        }
                        else {
                            roots_.PushBack(remembered);
                            remembered = nullptr;
                        }
                    }
                    continue;
                }
                // i >= roots.Size() && i >= other.roots.Size()
                if (remembered != nullptr) {
                    if (remembered->degree_ == i) {
                        roots_.PushBack(remembered);
                        remembered = nullptr;
                    }
                } else {
                    break;
                }
            }
        }
        other.Clear();
    }


    void MergeWithCopy(const BinomialHeap<KeyType> &other) {
        auto other_copy = new BinomialHeap<KeyType>(other);
        Merge(*other_copy);
    }

    void Clear() {
        roots_.Clear();
        size_ = 0;
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

private:
    template <typename _KeyType>
    struct node {
        node() : children_(), degree_(0) {}

        node(const node<_KeyType> &other) : children_(), degree_(0) {
            copy(other);
        }

        void copy(const node<_KeyType> &other) {
            value_ = other.value_;
            degree_ = other.degree_;
            for (int i = 0; i < other.children_.Size(); ++i) {
                shared_ptr<node<_KeyType>> copied_node( new node<_KeyType> );
                copied_node->copy(*other.children_[i]);
                children_.PushBack(copied_node);
            }
        }
        _KeyType value_;
        int degree_;
        DynamicArray<shared_ptr<node<_KeyType>>> children_;
    };

    template <typename _KeyType>
    shared_ptr<node<_KeyType>> merge_trees(shared_ptr <node<_KeyType>> n1, shared_ptr <node<_KeyType>> n2) {
        assert(n1 != nullptr && n2 != nullptr && (n1->degree_ == n2->degree_));
        if (comp_(n2->value_, n1->value_)) {
            swap(n1, n2);
        }
        n1->children_.PushBack(n2);
        ++n1->degree_;
        return n1;
    }

    int find_index_of_min() const {
        assert(!Empty());
        int min_index = 0;
        while (roots_[min_index] == nullptr) {
            ++min_index;
        }
        for (int i = min_index; i < roots_.Size(); ++i) {
            if (roots_[i] != nullptr && comp_(roots_[i]->value_, roots_[min_index]->value_)) {
                min_index = i;
            }
        }
        return min_index;
    }

    bool (*comp_)(KeyType, KeyType);
    DynamicArray<shared_ptr<node<KeyType>>> roots_;
    size_t size_;
};

template <typename KeyType>
BinomialHeap<KeyType> MergeHeapsCopies(const BinomialHeap<KeyType> &b1, const BinomialHeap<KeyType> &b2) {
    BinomialHeap<KeyType> merged(b1);
    merged.MergeWithCopy(b2);
    return merged;
}

template <typename  KeyType>
BinomialHeap<KeyType>& Merge(BinomialHeap<KeyType> &b1, BinomialHeap<KeyType> &b2) {  //clears b1 & b2
    auto *merged = new BinomialHeap<KeyType>;
    merged->Merge(b2);
    merged->Merge(b1);
    return *merged;
}

#endif //BINOMIAL_HEAP_HPP
