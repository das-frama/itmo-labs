//
// Created by Andrey Galaktionov on 25.12.2020.
//

#ifndef ITMO_LABS_ARRAY_H
#define ITMO_LABS_ARRAY_H

#include <cassert>
#include <cstdlib>
#include <utility>

template<typename T>
class Array final {
public:
    Array() : Array(8) {}

    Array(int capacity) {
        assert(capacity >= 0 && capacity <= INT_MAX);

        reallocate_memory(capacity);
    }

    Array(const Array<T> &other) {
        reallocate_memory(other._capacity);

        _size = other._size;
        for (size_t i = 0; i < _size; i += 1) {
            _data[i] = other._data[i];
        }
    }

    ~Array() {
        clear();
        free(_data); // may be we should switch to ::operator delete()

        _data = nullptr;
        _capacity = 0;
    }

    // Push back.
    void insert(T &&value) {
        insert(_size, std::move(value));
    }

    void insert(int index, const T &value) {
        _size += 1;

        if (_size >= _capacity) {
            reallocate_memory(_capacity + _capacity / 2); // Growing in 1.5 times.
        }

        for (size_t i = _size - 1; i > index; i -= 1) {
            _data[i] = _data[i - 1];
        }
        _data[index] = value;
    }

    void insert(int index, T &&value) {
        _size += 1;

        if (_size >= _capacity) {
            reallocate_memory(_capacity + _capacity / 2); // Growing in 1.5 times.
        }

        for (size_t i = _size - 1; i > index; i -= 1) {
            _data[i] = std::move(_data[i - 1]);
        }
        _data[index] = std::move(value);
    }

    void remove(int index) {
        assert(index >= 0 && index < _size);

        if (_size <= 0) return;

        _size -= 1;
        _data[index].~T();
        for (size_t i = index; i < _size; i += 1) {
            _data[i] = std::move(_data[i + 1]);
        }
    }

    void clear() {
        for (size_t i = 0; i < _size; i += 1) {
            _data[i].~T();
        }
        _size = 0;
    }

    // Get position.
    const T &operator[](size_t index) const {
        assert(index < _size);

        return _data[index];
    };

    T &operator[](int index) {
        assert(index < _size);

        return _data[index];
    }

    size_t size() const {
        return _size;
    }

private:
    void reallocate_memory(int new_capacity) {
        // New block of memory.
        T *temp_data = (T *) malloc(new_capacity * sizeof(T)); // may be we should switch to ::operator new()

        // Decrease size if new capacity is lower than current size.
        if (new_capacity < _size) {
            _size = new_capacity;
        }

        // Move every item.
        for (size_t i = 0; i < _size; i += 1) {
            temp_data[i] = std::move(_data[i]); // we are not using memcpy due to class problems.
        }

        // Call destructor of every item.
        for (size_t i = 0; i < _size; i += 1) {
            _data[i].~T();
        }

        // Manage data.
        free(_data); // may be we should switch to ::operator delete()
        _data = temp_data;
        _capacity = new_capacity;
    }

private:
    T *_data = nullptr;

    size_t _size = 0;
    size_t _capacity = 0;

private:
    class Iterator {
        friend Array;

    public:
        Iterator(Array<T> &array) : _array(array) {}

        Iterator(Array<T> &&array) noexcept: _array(array) {}

        const T &get() const {
            return _array[_index];
        }

        void set(const T &value) {
            _array[_index] = value;
        }

        void insert(const T &value) {
            _array.insert(_index, value);
//            if (_array.size() > 0) {
//                _index += 1;
//            }
        }

        void remove() {
            _array.remove(_index);
        }

        void next() {
            _index += 1;
        }

        void prev() {
            _index -= 1;
        }

        void toIndex(int index) {
            _index = index;
        }

        bool hasNext() const {
            return _index < _array.size() - 1;
        }

        bool hasPrev() const {
            return _index > 0;
        }

    private:
        int _index = 0;
        Array<T> &_array;
    };

public:
    Iterator iterator() {
        return std::move(Iterator(*this));
    }

    const Iterator iterator() const {
        return std::move(Iterator(*this));
    }
};

#endif //ITMO_LABS_ARRAY_H
