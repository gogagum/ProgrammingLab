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
struct Node {
    _KeyType value;
    shared_ptr<Node> parent;
    shared_ptr<Node> child;
    shared_ptr<Node> left_sibling;
    shared_ptr<Node> right_sibling;
    int degree = 0;
    bool is_marked = false;
};

template <typename _KeyType>
void UniteLists(shared_ptr<Node<_KeyType>> first, shared_ptr<Node<_KeyType>> second) {
    if (second == nullptr || first == nullptr) {
        return;
    }
    auto L = first->left_sibling;
    auto R = second->right_sibling;
    second->right_sibling = first;
    first->left_sibling = second;
    L->right_sibling = R;
    R->left_sibling = L;
}

template <typename _KeyType>
void EraseFromList(shared_ptr<Node<_KeyType>> to_erase) {
    auto L = to_erase->left_sibling;
    auto R = to_erase->right_sibling;
    L->right_sibling = R;
    R->left_sibling = L;
}

template <typename KeyType>
class FibonacciHeap {
public:
    explicit FibonacciHeap(bool (*comp)(KeyType, KeyType) = default_comparator)
     : comp_(comp), size_(0), max_root_degree_(0) { }

    shared_ptr<Node<KeyType>> Insert(KeyType key) {
        shared_ptr<Node<KeyType>> new_node_ptr(new Node<KeyType>);
        new_node_ptr->value = key;
        if (Empty()) {
            root = new_node_ptr;
            root->left_sibling = root;
            root->right_sibling = root;
        } else {
            auto prev_right = root->right_sibling;
            root->right_sibling = new_node_ptr;
            new_node_ptr->left_sibling = root;
            new_node_ptr->right_sibling = prev_right;
            prev_right->left_sibling = new_node_ptr;
            if (comp_(key, root->value)) {
                root = new_node_ptr;
            }
        }
        ++size_;
        return new_node_ptr;
    }

    void Merge(FibonacciHeap<KeyType> &other) {
        if (other.Empty()) {
            return;
        }
        if (Empty()) {
            root = other.root;
            size_ = other.Size();
        } else {
            UniteLists(root, other.root);
            size_ += other.size_;
            if (comp_(root->left_sibling->value, root->value)) {
                root = root->left_sibling;
            }
        }
        other.size_ = 0;
        other.root = nullptr;
        max_root_degree_ = max(max_root_degree_, other.max_root_degree_);
    }

    KeyType GetRoot() const {
        assert(!Empty());
        return root->value;
    }

    KeyType ExtractRoot() {
        assert(!Empty());
        --size_;
        auto prev_root = root;
        UniteLists(root, root->child);
        root->child = nullptr;
        if (prev_root->right_sibling == prev_root) {
            prev_root->right_sibling = nullptr;
            prev_root->left_sibling = nullptr;
            root = nullptr;
            return prev_root->value;
        } else {
            root = root->right_sibling;
            EraseFromList(prev_root);
            Consolidate();
        }
        return prev_root->value;
    }

    void DecreaseKey(shared_ptr<Node<KeyType>> to_decrease, KeyType key) {
        to_decrease->value = key;
        if ( to_decrease->parent == nullptr
         || comp_(to_decrease->parent->value, key)) {
            if (comp_(to_decrease->value, root->value)) {
                root = to_decrease;
            }
            return;
        }
        auto parent = to_decrease->parent;
        Cut(to_decrease);
        CascadingCut(parent);
        if (comp_(to_decrease->value, root->value)) {
            root = to_decrease;
        }
    }

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

private:
    void Consolidate() {
        assert(!Empty());
        auto roots = new shared_ptr<Node<KeyType>>[size_];
        roots[root->degree] = root;
        shared_ptr<Node<KeyType>> current = root->right_sibling;
        if (comp_(current->value, root->value)) {
            root = current;
        }

        bool changed = true;

        while (changed) {
            if (roots[current->degree] == nullptr) {
                roots[current->degree] = current;
                current = current->right_sibling;
            } else {
                if (current != roots[current->degree]) {
                    auto conflict = roots[current->degree];
                    roots[current->degree] = nullptr;
                    auto add_to = current;
                    auto adding = conflict;
                    if (comp_(adding->value, add_to->value)) {
                        swap(add_to, adding);
                    }
                    EraseFromList(adding);
                    adding->left_sibling = adding;
                    adding->right_sibling = adding;
                    UniteLists(add_to->child, adding);
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

        current = root->right_sibling;
        while (current != root) {
            if (comp_(current->value, root->value)) {
                root = current;
            }
            current = current->right_sibling;
        }

        delete [] roots;
    }

    void Cut(shared_ptr<Node<KeyType>> to_cut) {
        if (to_cut != to_cut->right_sibling) {
            EraseFromList(to_cut);
        }
        if (to_cut->parent != nullptr) {
            --(to_cut->parent->degree);
            if (to_cut->parent->child == to_cut) {
                if (to_cut->right_sibling == to_cut) {
                    to_cut->parent->child = nullptr;
                } else {
                    to_cut->parent->child = to_cut->right_sibling;
                }
            }
        }
        to_cut->right_sibling = to_cut;
        to_cut->left_sibling = to_cut;
        to_cut->is_marked = false;

        to_cut->parent = nullptr;
        UniteLists(root, to_cut);
    }

    void CascadingCut(shared_ptr<Node<KeyType>> to_cut) {
        while ( to_cut->is_marked ) {
            Cut(to_cut);
            to_cut = to_cut->parent;
        }
        to_cut->is_marked = true;
    }

    int max_root_degree_;
    bool (*comp_)(KeyType, KeyType);
    shared_ptr<Node<KeyType>> root;
    size_t  size_;
};


#endif //FIBONACCIHEAP_HPP
