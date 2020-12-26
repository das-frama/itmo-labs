//
// Created by Andrey Galaktionov on 20.12.2020.
//

#include <iostream>
#include "gtest/gtest.h"
#include "list.h"

template<typename T>
void print_list(List<T> &l) {
    for (auto it = l.iterator(); it.hasNext(); it.next()) {
        std::cout << it.get() << " ";
    }
    std::cout << std::endl;
}

TEST(list, empty) {
    List<int> l;

    ASSERT_EQ(l.size(), 0);
}

TEST(list, insert_head) {
    List<int> l;
    ASSERT_EQ(l.size(), 0);

    l.insertHead(1);
    ASSERT_EQ(l.head(), 1);
    ASSERT_EQ(l.size(), 1);

    l.insertHead(2);
    ASSERT_EQ(l.head(), 2);
    ASSERT_EQ(l.size(), 2);

    l.insertHead(3);
    ASSERT_EQ(l.head(), 3);
    ASSERT_EQ(l.size(), 3);

    l.insertHead(4);
    ASSERT_EQ(l.head(), 4);
    ASSERT_EQ(l.size(), 4);

    l.insertHead(5);
    ASSERT_EQ(l.head(), 5);
    ASSERT_EQ(l.size(), 5);
}

TEST(list, insert_tail) {
    List<int> l;
    ASSERT_EQ(l.size(), 0);

    l.insertTail(1);
    ASSERT_EQ(l.tail(), 1);
    ASSERT_EQ(l.size(), 1);

    l.insertTail(2);
    ASSERT_EQ(l.tail(), 2);
    ASSERT_EQ(l.size(), 2);

    l.insertTail(3);
    ASSERT_EQ(l.tail(), 3);
    ASSERT_EQ(l.size(), 3);

    l.insertTail(4);
    ASSERT_EQ(l.tail(), 4);
    ASSERT_EQ(l.size(), 4);

    l.insertTail(5);
    ASSERT_EQ(l.tail(), 5);
    ASSERT_EQ(l.size(), 5);
}

TEST(list, remove_head) {
    List<int> l;
    l.insertHead(1);
    l.insertHead(2);
    l.insertHead(3);
    l.insertHead(4);
    l.insertHead(5);
    ASSERT_EQ(l.head(), 5);
    ASSERT_EQ(l.size(), 5);
    l.removeHead();
    ASSERT_EQ(l.head(), 4);
    ASSERT_EQ(l.size(), 4);
    l.removeHead();
    ASSERT_EQ(l.head(), 3);
    ASSERT_EQ(l.size(), 3);
    l.removeHead();
    ASSERT_EQ(l.head(), 2);
    ASSERT_EQ(l.size(), 2);
    l.removeHead();
    ASSERT_EQ(l.head(), 1);
    ASSERT_EQ(l.size(), 1);
    l.removeHead();
    ASSERT_EQ(l.size(), 0);
}

TEST(list, remove_tail) {
    List<int> l;
    l.insertTail(1);
    l.insertTail(2);
    l.insertTail(3);
    l.insertTail(4);
    l.insertTail(5);
    ASSERT_EQ(l.tail(), 5);
    ASSERT_EQ(l.size(), 5);
    l.removeTail();
    ASSERT_EQ(l.tail(), 4);
    ASSERT_EQ(l.size(), 4);
    l.removeTail();
    ASSERT_EQ(l.tail(), 3);
    ASSERT_EQ(l.size(), 3);
    l.removeTail();
    ASSERT_EQ(l.tail(), 2);
    ASSERT_EQ(l.size(), 2);
    l.removeTail();
    ASSERT_EQ(l.tail(), 1);
    ASSERT_EQ(l.size(), 1);
    l.removeTail();
    ASSERT_EQ(l.size(), 0);
}

TEST(list_iterator, next) {
    List<int> l;
    for (int i = 0; i < 10; i += 1) {
        l.insertTail(i * 2);
    }
    int i = 0;
    for (auto it = l.iterator(); it.hasNext(); it.next()) {
        ASSERT_EQ(it.get(), i * 2);
        i += 1;
    }
}

TEST(list_iterator, prev) {
    List<int> l;
    for (int i = 0; i < 10; i += 1) {
        l.insertTail(i * 2);
    }
    for (auto it = l.iterator(); it.hasNext(); it.next());

    int i = 9;
    for (auto it = l.iterator(); it.hasPrev(); it.prev()) {
        ASSERT_EQ(it.get(), i * 2);
        i -= 1;
    }
}

TEST(list_iterator, insert) {
    List<int> l;
    auto it = l.iterator();
    it.insert(4);
    it.insert(1);
    it.insert(0);
    it.next();
    it.next();
    it.insert(3);
    it.insert(2);
    print_list(l);
    ASSERT_EQ(l.size(), 5);
    int i = 0;
    for (auto it2 = l.iterator(); !it2.hasNext(); it2.next(), i++) {
        ASSERT_EQ(it2.get(), i);
    }
}

// Iterator remove.
TEST(list_iterator, remove) {
    List<int> l;
    l.insertTail(1);
    l.insertTail(2);
    l.insertTail(3);
    l.insertTail(4);
    l.insertTail(5);
    ASSERT_EQ(l.size(), 5);

    auto it = l.iterator();
    it.remove();
    ASSERT_EQ(l.size(), 4);

    it.remove();
    ASSERT_EQ(l.size(), 3);

    it.remove();
    ASSERT_EQ(l.size(), 2);

    it.remove();
    ASSERT_EQ(l.size(), 1);

    it.remove();
    ASSERT_EQ(l.size(), 0);
}

TEST(list_iterator, class_test) {
    struct Vector2 {
        int x, y;

        Vector2(int x, int y) : x(x), y(y) {}
    };

    List<Vector2 *> l;
    for (int i = 0; i < 1000; i += 1) {
        l.insertTail(new Vector2(i, i));
    }
    l.insertTail(nullptr);
    ASSERT_EQ(l.size(), 1001);


    for (auto it = l.iterator(); it.hasNext(); it.next()) {
        delete it.get();
    }
    ASSERT_EQ(l.size(), 1001);
}
