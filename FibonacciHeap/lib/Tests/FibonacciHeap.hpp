//
// Created by gogagum on 24.11.18.
//

#ifndef FIBONACCIHEAP_HPP
#define FIBONACCIHEAP_HPP

#include <memory>
#include <cstddef>
#include <algorithm>

using std::shared_ptr;
using std::max;

template <typename  T>
bool default_comparator(T value1, T value2) {
    return value1 < value2;
}

template <typename _KeyType>
struct node {
    _KeyType value;
    shared_ptr<node> parent;
    shared_ptr<node> child;
    shared_ptr<node> left;
    shared_ptr<node> right;
    int degree = 0;
    bool mark = false;
};

template <typename _KeyType>
void unite_lists(shared_ptr<node<_KeyType>> first, shared_ptr<node<_KeyType>> second) {
    if (second == nullptr || first == nullptr) {
        return;
    }
    auto L = first->left;
    auto R = second->right;
    second->right = first;
    first->left = second;
    L->right = R;
    R->left = L;
}

template <typename _KeyType>
void erase_from_list(shared_ptr<node<_KeyType>> to_erase) {
    auto L = to_erase->left;
    auto R = to_erase->right;
    L->right = R;
    R->left = L;
}

template <typename KeyType>
class FibonacciHeap {
public:
    explicit FibonacciHeap(bool (*comp)(KeyType, KeyType) = default_comparator)
     : comp_(comp), size_(0), max_root_degree_(0) { }

    shared_ptr<node<KeyType>> insert(KeyType key) {
        shared_ptr<node<KeyType>> new_node_ptr(new node<KeyType>);
        new_node_ptr->value = key;
        if (empty()) {
            root = new_node_ptr;
            root->left = root;
            root->right = root;
        } else {
            auto prev_right = root->right;
            root->right = new_node_ptr;
            new_node_ptr->left = root;
            new_node_ptr->right = prev_right;
            prev_right->left = new_node_ptr;
            if (comp_(key, root->value)) {
                root = new_node_ptr;
            }
        }
        ++size_;
        return new_node_ptr;
    }

    void merge(FibonacciHeap<KeyType> &other) {
        if (other.empty()) {
            return;
        }
        if (empty()) {
            root = other.root;
            size_ = other.size();
        } else {
            unite_lists(root, other.root);
            size_ += other.size_;
            if (comp_(root->left->value, root->value)) {
                root = root->left;
            }
        }
        other.size_ = 0;
        other.root = nullptr;
        max_root_degree_ = max(max_root_degree_, other.max_root_degree_);
    }

    KeyType get_root() const {
        assert(!empty());
        return root->value;
    }

    KeyType extract_root() {
        assert(!empty());
        --size_;
        auto prev_root = root;
        unite_lists(root, root->child);
        root->child = nullptr;
        if (prev_root->right == prev_root) {
            prev_root->right = nullptr;
            prev_root->left = nullptr;
            root = nullptr;
            return prev_root->value;
        } else {
            root = root->right;
            erase_from_list(prev_root);
            consolidate();
        }
        return prev_root->value;
    }

    void decrease_key(shared_ptr<node<KeyType>> to_decrease, KeyType key) {
        to_decrease->value = key;
        if ( to_decrease->parent == nullptr
         || comp_(to_decrease->parent->value, key)) {
            if (comp_(to_decrease->value, root->value)) {
                root = to_decrease;
            }
            return;
        }
        auto parent = to_decrease->parent;
        cut(to_decrease);
        cascading_cut(parent);
        if (comp_(to_decrease->value, root->value)) {
            root = to_decrease;
        }
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

private:
    void consolidate() {
        assert(!empty());
        auto roots = new shared_ptr<node<KeyType>>[size_];
        roots[root->degree] = root;
        shared_ptr<node<KeyType>> current = root->right;
        if (comp_(current->value, root->value)) {
            root = current;
        }

        bool changed = true;

        while (changed) {
            if (roots[current->degree] == nullptr) {
                roots[current->degree] = current;
                current = current->right;
            } else {
                if (current != roots[current->degree]) {
                    auto conflict = roots[current->degree];
                    roots[current->degree] = nullptr;
                    auto add_to = current;
                    auto adding = conflict;
                    if (comp_(adding->value, add_to->value)) {
                        swap(add_to, adding);
                    }
                    erase_from_list(adding);
                    adding->left = adding;
                    adding->right = adding;
                    unite_lists(add_to->child, adding);
                    if (add_to->child == nullptr) {
                        add_to->child = adding;
                    }
                    adding->parent = add_to;
                    ++(add_to->degree);
                    current = add_to;
                } else {
                    changed = false;
                }
            }
            if (comp_(current->value, root->value)) {
                root = current;
            }
        }

        current = root->right;
        while (current != root) {
            if (comp_(current->value, root->value)) {
                root = current;
            }
            current = current->right;
        }

        delete [] roots;
    }

    void cut(shared_ptr<node<KeyType>> to_cut) {
        if (to_cut != to_cut->right) {
            erase_from_list(to_cut);
        }
        if (to_cut->parent != nullptr) {
            --(to_cut->parent->degree);
            if (to_cut->parent->child == to_cut) {
                if (to_cut->right == to_cut) {
                    to_cut->parent->child = nullptr;
                } else {
                    to_cut->parent->child = to_cut->right;
                }
            }
        }
        to_cut->right = to_cut;
        to_cut->left = to_cut;
        to_cut->mark = false;

        to_cut->parent = nullptr;
        unite_lists(root, to_cut);
    }

    void cascading_cut(shared_ptr<node<KeyType>> to_cut) {
        while ( to_cut->mark ) {
            cut(to_cut);
            to_cut = to_cut->parent;
        }
        to_cut->mark = true;
    }

    int max_root_degree_;
    bool (*comp_)(KeyType, KeyType);
    shared_ptr<node<KeyType>> root;
    size_t  size_;
};


#endif //FIBONACCIHEAP_HPP
