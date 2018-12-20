//
// Created by gogagum on 26.10.18.
//

#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <glob.h>
#include <cassert>

template <typename T>
class DynamicArray {
public:
    explicit DynamicArray (size_t size = 0) : size_(size), buffer_size_(1) {
        assert(size >= 0);
        while (buffer_size_ < size) {
            buffer_size_ *= 2;
        }
        buffer_ = new T[buffer_size_];
    }

    DynamicArray (size_t size, T* array) : DynamicArray(size) {
        for (int i = 0; i < size; ++i) {
            buffer_[i] = array[i];
        }
    }

    DynamicArray (size_t size, const T &element) : DynamicArray(size) {
        for (int i = 0; i < size; ++i) {
            buffer_[i] = element;
        }
    }

    template <typename Iterator>
    DynamicArray (Iterator it1, Iterator it2) : DynamicArray() {
        for (auto i = it1; i < it2; ++i) {
            push_back(*i);
        }
    }

    DynamicArray (const DynamicArray<T> &otherArray) : size_(otherArray.size_), buffer_size_(otherArray.buffer_size_) {
        buffer_ = new T[buffer_size_];
        for (int i = 0; i < size_; ++i) {
            buffer_[i] = otherArray.buffer_[i];
        }
    }

    T& operator [] (int index) {
        assert(index >= 0 && index < size_);
        return buffer_[index];
    }

    const T& operator [] (int index) const {
        assert(index >= 0 && index < size_);
        return buffer_[index];
    };

    void push_back (T new_element) {
        if (size_ == buffer_size_) {
            T* new_buffer = new T[2 * buffer_size_];
            for (int i = 0; i < buffer_size_; ++i) {
                new_buffer[i] = buffer_[i];
            }
            buffer_size_ *= 2;
            delete [] buffer_;
            buffer_ = new_buffer;
        }
        buffer_[size_] = new_element;
        ++size_;
    }

    T pop_back() {
        T to_return = buffer_[size_ - 1];
        --size_;
        if (size_ > 0 && size_ * 4 < buffer_size_) {
            buffer_size_ /= 2;
            T* new_buffer = new T[buffer_size_];
            for (int i = 0; i < size_; ++i) {
                new_buffer[i] = buffer_[i];
            }
            delete [] buffer_;
            buffer_ = new_buffer;
        }
        return to_return;
    }

    void erase (int index) {
        assert(index >= 0 && index < size_);
        --size_;
        if (size_ * 4 < buffer_size_ && buffer_size_ != 1) {
            buffer_size_ /= 2;
            T* new_buffer = new T[buffer_size_];
            for (int i = 0; i < index; ++i) {
                new_buffer[i] = buffer_[i];
            }
            for (int i = index; i < size_; ++i) {
                new_buffer[i] = buffer_[i + 1];
            }
            delete [] buffer_;
            buffer_ = new_buffer;
        }
        else {
            for (int i = index; i < size_; ++i) {
                buffer_[i] = buffer_[i + 1];
            }
        }
    }

    void clear() {
        delete [] buffer_;
        buffer_size_ = 1;
        buffer_ = new T[buffer_size_];
        size_ = 0;
    }

    size_t size () const {
        return size_;
    }

    ~DynamicArray () {
        delete [] buffer_;
    }

private:
    T* buffer_;
    size_t size_;
    size_t buffer_size_;
};

template <typename T>
bool operator == (const DynamicArray<T> &d1, const DynamicArray<T> &d2) {
    if (d1.size() != d2.size()) {
        return false;
    } else {
        for (int i = 0; i < d1.size(); ++i) {
            if (d1[i] != d2[i]) {
                return false;
            }
        }
    }
    return true;
}


#endif //DYNAMICARRAY_H
