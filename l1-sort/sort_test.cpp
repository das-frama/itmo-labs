//
// Created by Andrey Galaktionov on 19.12.2020.
//

#include <stdlib.h>
#include <time.h>
#include "gtest/gtest.h"
#include "sort.h"

#define COMP_LESS  [](int a, int b) { return a < b; }
#define COMP_GREAT [](int a, int b) { return a > b; }

template<typename T>
void test_array_eq(T *a, T *b, int n) {
    for (int i = 0; i < n; i += 1) {
        ASSERT_EQ(a[i], b[i]);
    }
}

template<typename T, typename Compare>
void test_array_sorted(T *first, T *last, Compare comp) {
    for (auto i = first; i < last - 1; i += 1) {
        EXPECT_TRUE(comp(*i, *(i + 1)) || !comp(*(i + 1), *i));
    }
}

template<typename T>
void print_array(T *first, int n) {
    for (int i = 0; i < n; i += 1) {
        printf("%d ", *(first + i));
    }
    printf("\n");
}

struct Beatle {
    int size;
};

// Empty array.
TEST(test_sort, test_empty) {
    int a[10] = {};
    sort(a, a, COMP_LESS);

    test_array_sorted(a, a + 10, COMP_LESS);
}

// One element.
TEST(test_sort, test_one) {
    int a[] = {1};
    sort(a, a, COMP_LESS);
    test_array_sorted(a, a + 1, COMP_LESS);

    int b[] = {1};
    test_array_eq(a, b, 1);
}

// Two elements.
TEST(test_sort, test_two) {
    int a[] = {42, 10};
    sort(a, a + 1, COMP_LESS);
    test_array_sorted(a, a + 2, COMP_LESS);
}

// Six elements.
TEST(test_sort, test_six) {
    int a[] = {15, 4, 42, 23, 8, 16};
    sort(a, a + 5, COMP_LESS);
    test_array_sorted(a, a + 5, COMP_LESS);
}

// Duplicates elements.
TEST(test_sort, test_duplicates) {
    int a[] = {16, 1, 1, 2, 8, 4, 4, 8, 2, 16};
    sort(a, a + 9, COMP_LESS);
    test_array_sorted(a, a + 9, COMP_LESS);
}

// All duplicates elements.
TEST(test_sort, test_all_duplicates) {
    int a[] = { 5, 5, 5, 5, 7, 5 };
    sort(a, a + 5, COMP_LESS);
    test_array_sorted(a, a + 5, COMP_LESS);
}

// Big elements.
TEST(test_sort, test_big) {
    int n = 10000;
    int *a = new int[n];

    srand(time(NULL));
    for (int i = 0; i < n; i += 1) {
        a[i] = rand() % n;
    }

    sort(a, a + n - 1, [](int a, int b) { return a < b; });
    test_array_sorted(a, a + n - 1, [](int a, int b) { return a < b; });

    delete[] a;
}

// Class test.
TEST(test_sort, test_class) {
    int n = 50; // 50 жуков.
    auto *a = new Beatle[n];

    srand(time(NULL));
    for (int i = 0; i < n; i += 1) {
        a[i].size = rand() % 255 + 50;
    }

    sort(a, a + n - 1, [](const Beatle &a, const Beatle &b) { return a.size < b.size; });
    test_array_sorted(a, a + n - 1, [](const Beatle &a, const Beatle &b) { return a.size < b.size; });

    delete[] a;
}