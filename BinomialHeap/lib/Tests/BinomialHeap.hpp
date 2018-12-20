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
                roots_.push_back(nodes_to_merge[0]);
                delete [] nodes_to_merge;
            }
            else {
                roots_.push_back(nullptr);
            }
        }
    }

    BinomialHeap(const BinomialHeap<KeyType> &other)
     : size_(other.size_), roots_(), comp_(other.comp_) {
        for (int i = 0; i < other.roots_.size(); ++i) {
            if (other.roots_[i] != nullptr) {
                shared_ptr<node<KeyType>> copied_node(new node<KeyType>);
                copied_node->copy(*(other.roots_[i]));
                roots_.push_back(copied_node);
            }
            else {
                roots_.push_back(nullptr);
            }
        }
    }

    void insert(KeyType key) {
        BinomialHeap<KeyType> to_merge(comp_);
        shared_ptr<node<KeyType>> one_element_node(new node<KeyType>);
        one_element_node->value_ = key;
        one_element_node->degree_ = 0;
        ++to_merge.size_;
        to_merge.roots_.push_back(one_element_node);
        merge(to_merge);
    }

    KeyType get_min() const {
        assert (!empty());
        return roots_[find_index_of_min()]->value_;
    }

    KeyType extract_min() {
        assert(!empty());
        auto min_index = find_index_of_min();
        auto min_ptr = roots_[min_index];
        BinomialHeap<KeyType> heap_from_children;
        for (int i = 0; i < min_ptr->children_.size(); ++i) {
            heap_from_children.roots_.push_back(min_ptr->children_[i]);
        }
        KeyType to_return = min_ptr->value_;
        roots_[min_index] = nullptr;
        merge(heap_from_children);
        --size_;
        return to_return;
    }

    void merge(BinomialHeap<KeyType> &other) { //clears other
        assert(comp_ == other.comp_);
        shared_ptr<node<KeyType>> remembered = nullptr;
        size_ += other.size();
        auto max_size_of_new_tree = max(roots_.size(), other.roots_.size()) + 1;

        for (int i = 0; i < max_size_of_new_tree; ++i) {
            if (i < min(roots_.size(), other.roots_.size())) {
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
                if (i < roots_.size()) {
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
                if (i < other.roots_.size()) {
                    if (remembered == nullptr) {
                        roots_.push_back(other.roots_[i]);
                    }
                    else {
                        if (other.roots_[i] != nullptr) {
                            remembered = merge_trees(remembered, other.roots_[i]);
                            roots_.push_back(nullptr);
                        }
                        else {
                            roots_.push_back(remembered);
                            remembered = nullptr;
                        }
                    }
                    continue;
                }
                // i >= roots.size() && i >= other.roots.size()
                if (remembered != nullptr) {
                    if (remembered->degree_ == i) {
                        roots_.push_back(remembered);
                        remembered = nullptr;
                    }
                } else {
                    break;
                }
            }
        }
        other.clear();
    }


    void merge_with_copy(const BinomialHeap<KeyType> &other) {
        auto other_copy = new BinomialHeap<KeyType>(other);
        merge(*other_copy);
    }

    void clear() {
        roots_.clear();
        size_ = 0;
    }

    bool empty () const {
        return size_ == 0;
    }

    size_t size () const {
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
            for (int i = 0; i < other.children_.size(); ++i) {
                shared_ptr<node<_KeyType>> copied_node( new node<_KeyType> );
                copied_node->copy(*other.children_[i]);
                children_.push_back(copied_node);
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
        n1->children_.push_back(n2);
        ++n1->degree_;
        return n1;
    }

    int find_index_of_min() const {
        assert(!empty());
        int min_index = 0;
        while (roots_[min_index] == nullptr) {
            ++min_index;
        }
        for (int i = min_index; i < roots_.size(); ++i) {
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
BinomialHeap<KeyType> merge_heaps_copies(const BinomialHeap<KeyType>& b1, const BinomialHeap<KeyType>& b2) {
    BinomialHeap<KeyType> merged(b1);
    merged.merge_with_copy(b2);
    return merged;
}

template <typename  KeyType>
BinomialHeap<KeyType>& merge(BinomialHeap<KeyType>& b1, BinomialHeap<KeyType>& b2) {  //clears b1 & b2
    auto *merged = new BinomialHeap<KeyType>;
    merged->merge(b2);
    merged->merge(b1);
    return *merged;
}

#endif //BINOMIAL_HEAP_HPP
