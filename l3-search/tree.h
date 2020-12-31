//
// Created by Andrey Galaktionov on 20.12.2020.
//

#ifndef ITMO_LABS_TREE_H
#define ITMO_LABS_TREE_H

#include <iostream>

template<typename K, typename V>
class AVLTree final {
private:
    struct Node {
        K key;
        V value;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;
        int height = 0;

        Node(K key, V value, Node *parent) : key(key), value(value), parent(parent) {
            left = nullptr;
            right = nullptr;
            height = 1;
        }

        Node *next() {
            auto node = this;

            if (node->right != nullptr) {
                node = node->right;
                while (node->left) {
                    node = node->left;
                }
            } else {
                while (node->parent != nullptr && node->parent->right == node) {
                    node = node->parent;
                }
                node = node->parent;
            }

            return node;
        }

        void deleteRecursive() {
            if (right != nullptr) {
                right->deleteRecursive();
                delete right;
            }
            if (left != nullptr) {
                left->deleteRecursive();
                delete left;
            }
        }
    };

public:
    AVLTree() {
        std::cout << "AVLTree constructor" << std::endl;
        _root = nullptr;
    }

    AVLTree(const AVLTree<K, V> &other) = delete;

    AVLTree(AVLTree<K, V> &&other) = delete;

    ~AVLTree() {
        std::cout << "AVLTree destructor" << std::endl;
        if (_root != nullptr) {
            _root->deleteRecursive();
            delete _root;
        }
    }

    void put(const K &key, const V &value) {
        _root = _insert(_root, _root, key, value);
    }

    void remove(const K &key) {
        _root = _remove(_root, key);
    }

    bool contains(const K &key) {
        auto node = _search(key);

        if (node == nullptr) {
            return false;
        }

        return node->key == key;
    }

    const V &operator[](const K &key) const {
        auto node = _search(key);
        if (node == nullptr) {
            return V();
        }

        return node->value;
    }

    V &operator[](const K &key) {
        auto node = _search(key);
        if (node == nullptr) {
            node = _insert(_root, _root->parent, key, V());
        }

        return node->value;
    }

    size_t size() const {
        return _size;
    }

private:
    Node *_first() {
        Node *node = _root;
        for (; node->left; node = node->left);
        return node;
    }

    Node *_insert(Node *node, Node *parent, const K &key, const V &value) {
        if (node == nullptr) {
            _size += 1;
            return new Node(key, value, parent);
        }

        if (key < node->key) {
            node->left = _insert(node->left, node, key, value);
        } else if (key > node->key) {
            node->right = _insert(node->right, node, key, value);
        } else {
            node->value = value;
            return node;
        }

        node->height = 1 + _max(_height(node->left), _height(node->right));

        int balance = this->_balance(node);

        // left left
        if (balance > 1 && key < node->left->key) {
            return _rightRotate(node);
        }

        // right right
        if (balance < -1 && key > node->right->key) {
            return _leftRotate(node);
        }

        // left right
        if (balance > 1 && key > node->left->key) {
            node->left = _leftRotate(node->left);
            _rightRotate(node);
        }

        // right left
        if (balance < -1 && key < node->right->key) {
            node->right = _rightRotate(node->right);
            _leftRotate(node);
        }

        return node;
    }

    Node *_remove(Node *node, const K &key) {
        if (node == nullptr) {
            _size -= 1;
            return node;
        }

        // In left subtree.
        if (key < node->key) {
            node->left = _remove(node->left, key);
        } else if (key > node->key) {
            node->right = _remove(node->right, key);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                Node *tmp = node->left ? node->left : node->right;

                // No child.
                if (tmp == nullptr) {
                    tmp = node;
                    node = nullptr;
                } else { // One child.
                    *node = *tmp;
                }
                free(tmp);
            } else {
                Node *tmp = _min_value_node(node->right);

                node->key = tmp->key;
                node->right = _remove(node->right, tmp->key);
            }
        }

        if (node == nullptr) {
            _size -= 1;
            return node;
        }

        // Update height.
        node->height = 1 + _max(_height(node->left), _height(node->right));

        // Balance.
        int balance = _balance(node);

        // left left
        if (balance > 1 && key < node->left->key) {
            return _rightRotate(node);
        }

        // right right
        if (balance < -1 && key > node->right->key) {
            return _leftRotate(node);
        }

        // left right
        if (balance > 1 && key > node->right->key) {
            node->left = _leftRotate(node->left);
            _rightRotate(node);
        }

        // right left
        if (balance < -1 && key < node->left->key) {
            node->right = _rightRotate(node->right);
            _leftRotate(node);
        }

        return _root;
    }

    Node *_leftRotate(Node *a) {
        Node *b = a->right;
        Node *c = b->left;

        // Rotation.
        b->left = a;
        a->right = c;

        // Parent.
        if (c) {
            c->parent = a;
        }
        b->parent = a->parent;
        a->parent = b;

        // Heights update.
        a->height = 1 + _max(_height(a->left), _height(a->right));
        b->height = 1 + _max(_height(b->left), _height(b->right));

        return b;
    }

    Node *_rightRotate(Node *b) {
        Node *a = b->left;
        Node *c = a->right;

        // Rotation.
        a->right = b;
        b->left = c;

        // Parent.
        if (c) {
            c->parent = b;
        }
        a->parent = b->parent;
        b->parent = a;

        // Heights update.
        b->height = 1 + _max(_height(b->left), _height(b->right));
        a->height = 1 + _max(_height(a->left), _height(a->right));

        return a;
    }

    Node *_search(const K &key) {
        if (_root == nullptr) {
            return _root;
        }

        Node *node = _root;
        while (true) {
            if (node == nullptr || node->key == key) {
                return node;
            } else if (node->key < key) {
                node = node->right;
            } else if (node->key > key) {
                node = node->left;
            }
        }
    }

private:
    size_t _size = 0;
    Node *_root = nullptr;

    K _key(const Node *node) const {
        if (node == nullptr) {
            return K();
        }

        if (node->left != nullptr) {
            return node->left->key;
        }

        if (node->right != nullptr) {
            return node->right->key;
        }

        return K();
    }

    int _max(int a, int b) {
        return a > b ? a : b;
    }

    int _height(const Node *node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int _balance(const Node *node) {
        if (node == nullptr) {
            return 0;
        }

        return _height(node->left) - _height(node->right);
    }

    Node *_min_value_node(Node *node) {
        Node *current = node;
        while (current->left != nullptr) {
            current = current->left;
        }

        return current;
    }

    class Iterator {
        friend AVLTree;

    public:
        explicit Iterator(AVLTree<K, V> &tree) : _tree(tree) {
            _node = tree._first();
        }

        explicit Iterator(AVLTree<K, V> &&tree) noexcept: _tree(tree) {}

        const K &key() const {
            return _node->key;
        }

        const V &get() const {
            return _node->value;
        }

        void set(const V &value) {
            _node->value = value;
        }

        void next() {
            _node = _node->next();
        }

        bool isEnd() const {
            return _node == nullptr;
        }

    private:
        Node *_node = nullptr;
        AVLTree<K, V> &_tree;
    };

public:
    Iterator iterator() {
        return std::move(Iterator(*this));
    }

    Iterator iterator() const {
        return std::move(Iterator(*this));
    }
};

#endif //ITMO_LABS_TREE_H
