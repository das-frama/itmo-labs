//
// Created by Andrey Galaktionov on 02.12.2020.
//

#ifndef ITMO_LABS_LIST_H
#define ITMO_LABS_LIST_H

#include <iostream>

#define CACHE_LINE_SIZE 64
#define MAX_CHUNK_SIZE 4

template<typename T>
class List final {
private:
    struct Node {
    public:
        Node *prev = nullptr;
        Node *next = nullptr;
        T *data = nullptr;
        size_t size = 0;

        Node(Node *prev1, Node *next1) : prev(prev1), next(next1), size(0) {
            data = new T[MAX_CHUNK_SIZE];
            if (prev == nullptr) {
                prev = this;
            } else {
                prev->next = this;
            }

            if (next == nullptr) {
                next = this;
            } else {
                next->prev = this;
            }
        }

        ~Node() {
            if (!isTail()) {
                prev->next = next;
            }
            if (!isHead()) {
                next->prev = prev;
            }
        }

        bool isFull() const {
            return size >= MAX_CHUNK_SIZE;
        }

        bool isEmpty() const {
            return size == 0;
        }

        bool isHead() const {
            return this == prev;
        }

        bool isTail() const {
            return this == next;
        }
    };

public:
    List() {
        _head = new Node(nullptr, nullptr);
        _tail = _head;
        _size = 0;
    }

    List(const List &other) = delete;

    ~List() {
        while (_head != _tail) {
            auto temp = _head;
            _head = _head->next;
            delete temp;
        }
        delete _head;
    }

    void insertHead(T &&value) {
        if (_head->isFull()) {
            _head = new Node(nullptr, _head);
        }

        for (size_t i = _head->size; i > 0; i -= 1) {
            _head->data[i] = std::move(_head->data[i - 1]);
        }

        _head->data[0] = value;
        _head->size += 1;
        _size += 1;
    }

    void insertTail(const T &value) {
        if (_tail->isFull()) {
            _tail = new Node(_tail, nullptr);
        }
        _tail->data[_tail->size] = value;
        _tail->size += 1;
        _size += 1;
    }

    void removeHead() {
        // Remove head.
        if (_head->size > 0) {
            _head->data[0].~T();
            _head->size -= 1;
            for (size_t i = 0; i < _head->size; i += 1) {
                _head->data[i] = std::move(_head->data[i + 1]);
            }
        }

        if (_head->isEmpty() && !_head->isTail()) {
            _head = _head->next;
            delete _head->prev;
            _head->prev = _head;
        }

        _size -= 1;
    }

    void removeTail() {
        // Remove tail.
        if (_tail->size > 0) {
            _tail->data[_tail->size - 1].~T();
            _tail->size -= 1;
        }

        if (_tail->isEmpty() && !_tail->isHead()) {
            _tail = _tail->prev;
            delete _tail->next;
            _tail->next = _tail;
        }

        _size -= 1;
    }

    const T &head() const {
        return _head->data[0];
    }

    const T &tail() const {
        return _tail->data[_tail->size - 1];
    }

    int size() const {
        return _size;
    }

private:
    Node *_head = nullptr;
    Node *_tail = nullptr;

    size_t _size = 0;

private:
    class Iterator {
    public:
        explicit Iterator(List<T> *list) : _list(list), _node(list->_head), _index(0) {}

        const T &get() const {
            return _node->data[_index];
        }

        void set(const T &value) {
            _node->data[_index] = value;
        }

        void insert(const T &value) {
            if (_node->isFull()) {
                if (_node->isTail()) {
                    _list->_tail = new Node(_node, nullptr);
                } else {
                    _node->next = new Node(_node, _node->next);
                }
                for (size_t i = _index; i < MAX_CHUNK_SIZE; i += 1) {
                    _node->next->data[_node->next->size] = std::move(_node->data[i]);
                    _node->next->size += 1;
                    _node->size -= 1;
                    _node->data[_node->size].~T();
                }
            }
            // Insert element.
            _node->size += 1;
            for (size_t i = _node->size - 1; i > _index; i -= 1) {
                _node->data[i] = std::move(_node->data[i - 1]);
            }
            _node->data[_index] = std::move(value);

            _list->_size += 1;
        }

        void remove() {
            // Remove element.
            if (!_node->isEmpty()) {
                _node->size -= 1;
                for (size_t i = _index; i < _node->size; i += 1) {
                    _node->data[i] = std::move(_node->data[i + 1]);
                }
                _list->_size -= 1;
            }

            // If node is empty...
            if (_node->isEmpty() && _node->isTail() && !_node->isHead()) {
                _list->_tail = _node->prev;
                delete _node;
                _node = _list->_tail;
                _node->next = _node;
                _index = _node->size - 1;
            }

            if (_index >= _node->size) {
                if (!_node->isTail()) {
                    _node = _node->next;
                    _index = 0;
                } else {
                    _index = _node->size - 1;
                }
            }
        }

        void next() {
            if (_index < _node->size - 1) {
                _index += 1;
                return;
            }
            if (_node->isTail()) {
                _index = _node->size;
            } else {
                _node = _node->next;
                _index = 0;
            }
        }

        void prev() {
            if (_index > 0) {
                _index -= 1;
                return;
            }

            if (_node->isHead()) {
                _index = -1;
            } else {
                _node = _node->prev;
                _index = _node->size - 1;
            }
        }

        bool hasNext() const {
            return _index < _node->size - 1 || !_node->isTail();
        }

        bool hasPrev() const {
            return _index > 0 || !_node->isHead();
        }

    private:
        int _index;
        Node *_node = nullptr;
        List<T> *_list = nullptr;
    };

public:
    Iterator iterator() {
        return Iterator(this);
    }
};

#endif //ITMO_LABS_LIST_H
