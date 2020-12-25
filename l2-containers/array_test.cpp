//
// Created by Andrey Galaktionov on 20.12.2020.
//

#include <iostream>
#include <ctime>
#include "gtest/gtest.h"
#include "array.h"

struct Vector2 {
    float x = 0.0f, y = 0.0f;
    int *memory_block = nullptr;

    Vector2() {
        memory_block = new int[5];
    };

    explicit Vector2(float scalar) : x(scalar), y(scalar) {
        memory_block = new int[5];
    }

    Vector2(float x, float y) : x(x), y(y) {
        memory_block = new int[5];
    }

    Vector2(const Vector2 &other) : x(other.x), y(other.y) {
        std::cout << "Copy of Vector2" << std::endl;
        memory_block = new int[5];
        memcpy(memory_block, other.memory_block, 5 * sizeof(int));
    };

    Vector2(Vector2 &&other) noexcept: x(other.x), y(other.y) {
        std::cout << "Move of Vector2" << std::endl;
        memory_block = other.memory_block;
        other.memory_block = nullptr;
    }

    ~Vector2() {
        std::cout << "Destroy of Vector2" << std::endl;
        delete[] memory_block;
        x = y = 0.0f;
    }

    Vector2 &operator=(const Vector2 &other) {
        std::cout << "Copy of Vector2" << std::endl;
        memory_block = new int[5];
        memcpy(memory_block, other.memory_block, 5 * sizeof(int));

        return *this;
    }

    Vector2 &operator=(Vector2 &&other) noexcept {
        std::cout << "Move of Vector2" << std::endl;
        x = other.x;
        y = other.y;
        memory_block = other.memory_block;
        other.memory_block = nullptr;

        return *this;
    }
};

template<typename T>
void print_array(const Array<T> &a) {
    for (size_t i = 0; i < a.size(); i += 1) {
        std::cout << a[i] << " ";
    }
    std::cout << "-----------------" << std::endl;
}

template<>
void print_array(const Array<Vector2> &a) {
    for (size_t i = 0; i < a.size(); i += 1) {
        std::cout << "x=" << a[i].x << " y=" << a[i].y << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

// Test empty.
TEST(test_array, empty_size) {
    Array<int> a;
    ASSERT_EQ(a.size(), 0);
}

// Test insert.
TEST(test_array, insert) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.insert(4);
    a.insert(5);
    a.insert(6);

    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a[2], 3);
    ASSERT_EQ(a[3], 4);
    ASSERT_EQ(a[4], 5);
    ASSERT_EQ(a[5], 6);
    ASSERT_EQ(a.size(), 6);
}

// Test insertion with index.
TEST(test_array, insert_with_index) {
    Array<int> a;
    a.insert(2);
    a.insert(0, 1);
    a.insert(1, 3);

    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 3);
    ASSERT_EQ(a[2], 2);
    ASSERT_EQ(a.size(), 3);
}

// Test remove.
TEST(test_array, remove) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    a.remove(1);
    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 3);

    a.remove(0);
    ASSERT_EQ(a[0], 3);

    ASSERT_EQ(a.size(), 1);
}

TEST(test_array, operator_brackets) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    ASSERT_EQ(a[0], 1);
    ASSERT_EQ(a[1], 2);
    ASSERT_EQ(a[2], 3);
}

TEST(test_array, size) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    ASSERT_EQ(a.size(), 3);
}

TEST(test_array, copy_constructor) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    Array<int> b = a;
    ASSERT_EQ(a.size(), b.size());

    a.remove(0);
    a.remove(0);
    a.remove(0);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(b.size(), 3);

    b.remove(0);
    b.remove(0);
    b.remove(0);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(b.size(), 0);
}

TEST(test_array, class_copy_constructor) {
    Array<Vector2> a;
    a.insert(Vector2(1.0f));
    a.insert(Vector2(2.0f));
    a.insert(Vector2(3.0f));

    Array<Vector2> b = a;
    ASSERT_EQ(a.size(), b.size());

    a.remove(0);
    a.remove(0);
    a.remove(0);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(b.size(), 3);

    b.remove(0);
    b.remove(0);
    b.remove(0);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(b.size(), 0);
}

TEST(test_array, class_test) {
    Array<Vector2> a;
    a.insert(Vector2(2.0f));
    a.insert(Vector2(3.0f));
    a.insert(Vector2(4.0f));
    a.insert(0, Vector2(1.0f));
    print_array(a);

    a.remove(3);
    print_array(a);
    a.remove(0);
    print_array(a);
    a.remove(1);
    print_array(a);
    a.remove(0);
    print_array(a);
}

TEST(test_iterator, get) {
    Array<int> a;
    a.insert(0);
    a.insert(1);
    a.insert(2);
    auto it = a.iterator();

    ASSERT_EQ(it.get(), 0);
}

TEST(test_iterator, set) {
    Array<int> a;
    a.insert(0);
    a.insert(1);
    a.insert(2);
    auto it = a.iterator();
    it.set(2);

    ASSERT_EQ(it.get(), 2);
}

TEST(test_iterator, insert) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    it.insert(2);
    it.insert(3);
    it.insert(4);
    it.insert(5);
    it.insert(6);
    it.insert(7);

    ASSERT_EQ(a[0], 7);
    ASSERT_EQ(a[1], 6);
    ASSERT_EQ(a[2], 5);
    ASSERT_EQ(a[3], 4);
    ASSERT_EQ(a[4], 3);
    ASSERT_EQ(a[5], 2);
    ASSERT_EQ(a[6], 1);
    ASSERT_EQ(a[7], 0);
    ASSERT_EQ(a.size(), 8);
}
//
TEST(test_iterator, remove) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(1);
    it.insert(2);
    it.insert(3);

    it.remove();
    it.remove();
    it.remove();

    ASSERT_EQ(a.size(), 0);
}

TEST(test_iterator, next) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    it.insert(2);

    ASSERT_EQ(it.get(), 2);
    it.next();
    ASSERT_EQ(it.get(), 1);
    it.next();
    ASSERT_EQ(it.get(), 0);
}

TEST(test_iterator, prev) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    it.insert(2);

    it.toIndex(2);
    ASSERT_EQ(it.get(), 0);
    it.prev();
    ASSERT_EQ(it.get(), 1);
    it.prev();
    ASSERT_EQ(it.get(), 2);
}

TEST(test_iterator, to_index) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    it.insert(2);

    it.toIndex(1);
    ASSERT_EQ(it.get(), 1);
}

TEST(test_iterator, has_next) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    ASSERT_TRUE(it.hasNext());
}

TEST(test_iterator, has_prev) {
    Array<int> a;
    auto it = a.iterator();
    it.insert(0);
    it.insert(1);
    it.toIndex(1);

    ASSERT_TRUE(it.hasPrev());
}

TEST(test_iterator, for_loop) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        a.insert(i + 1);
    }
    for (int i = 0; i < a.size(); i += 1) {
        a[i] *= 2;
    }
    int i = 2;
    for (auto it = a.iterator(); it.hasNext(); it.next()) {
        ASSERT_EQ(i, it.get());
        i += 2;
    }
}
