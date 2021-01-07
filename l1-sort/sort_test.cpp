//
// Created by Andrey Galaktionov on 19.12.2020.
//

#include <stdio.h>
#include <windows.h>
#include "gtest/gtest.h"
#include "sort.h"

#define COMPARE_LESS [](int a, int b) { return a < b; }
#define COMPARE_LESS_FLOAT [](float a, float b) { return a < b; }
#define COMPARE_LESS_CHAR [](char a, char b) { return a < b; }
#define COMPARE_GREAT [](int a, int b) { return a > b; }

#define N_PERFORMANCE 200000000

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

// Empty array.
TEST(test_sort, test_empty) {
    int a[10] = {};
    sort(a, a, COMPARE_LESS);

    test_array_sorted(a, a + 10, COMPARE_LESS);
}

// One element.
TEST(test_sort, test_one) {
    int a[] = {1};
    sort(a, a, COMPARE_LESS);
    test_array_sorted(a, a + 1, COMPARE_LESS);

    int b[] = {1};
    test_array_eq(a, b, 1);
}

// Two elements.
TEST(test_sort, test_two) {
    int a[] = {42, 10};
    sort(a, a + 1, COMPARE_LESS);
    test_array_sorted(a, a + 2, COMPARE_LESS);
}

// Six elements.
TEST(test_sort, test_six) {
    int a[] = {15, 4, 42, 23, 8, 16};
    sort(a, a + 5, COMPARE_LESS);
    test_array_sorted(a, a + 5, COMPARE_LESS);
}

// Duplicates elements.
TEST(test_sort, test_duplicates) {
    int a[] = {16, 1, 1, 2, 8, 4, 4, 8, 2, 16};
    sort(a, a + 9, COMPARE_LESS);
    test_array_sorted(a, a + 9, COMPARE_LESS);
    print_array(a, 10);
}

// All duplicates elements.
TEST(test_sort, test_all_duplicates) {
    int a[] = {5, 5, 5, 5, 5, 5};
    sort(a, a + 5, COMPARE_LESS);
    test_array_sorted(a, a + 5, COMPARE_LESS);
}

// Test reverse case.
TEST(test_sort, test_worst) {
    int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    sort(a, a + 9, COMPARE_LESS);
    test_array_sorted(a, a + 9, COMPARE_LESS);
}

// Test reverse case great.
TEST(test_sort, test_great) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sort(a, a + 9, COMPARE_GREAT);
    test_array_sorted(a, a + 9, COMPARE_GREAT);
}

// Test none case.
TEST(test_sort, test_none) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sort(a, a + 9, COMPARE_LESS);
    test_array_sorted(a, a + 9, COMPARE_LESS);
}

// Test floats.
TEST(test_sort, test_floats) {
    float a[] = {5.0, 4.0, 2.0, 4.0, 10.0, 1.0, 6.0, 9.0, 12.0, 8.0};
    sort(a, a + 9, COMPARE_LESS_FLOAT);
    test_array_sorted(a, a + 9, COMPARE_LESS_FLOAT);
}

// Test chars.
TEST(test_sort, test_chars) {
    float a[] = {'b', 'z', 'f', 'e', 'o', 'h', 'p', 'a', 'c', 'g'};
    sort(a, a + 9, COMPARE_LESS_CHAR);
    test_array_sorted(a, a + 9, COMPARE_LESS_CHAR);
}


// Big elements.
TEST(test_sort, test_big) {
    int n = 200000000;
    int *a = new int[n];

    srand(time(NULL));
    for (int i = 0; i < n; i += 1) {
        a[i] = rand() % n;
    }

    sort(a, a + n - 1, COMPARE_LESS);
    test_array_sorted(a, a + n - 1, COMPARE_LESS);

    delete[] a;
}

// Class test.
TEST(test_sort, test_class) {
    struct Beetle {
        int size;
    };

    const int N = 500; // 500 жуков.
    auto beetles = new Beetle[N];

    srand(time(NULL));
    for (int i = 0; i < N; i += 1) {
        beetles[i].size = rand() % 255 + 50;
    }

    sort(beetles, beetles + N - 1, [](const Beetle &a, const Beetle &b) { return a.size < b.size; });
    test_array_sorted(beetles, beetles + N - 1, [](const Beetle &a, const Beetle &b) { return a.size < b.size; });

    delete[] beetles;
}

// Performance test.
TEST(test_performance, test_qsort) {
    int *a = new int[N_PERFORMANCE];
    srand(time(NULL));
    for (int i = 0; i < N_PERFORMANCE; i += 1) {
        a[i] = rand() % N_PERFORMANCE;
    }

    // Init.
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    // Sort.
    sort(a, a + N_PERFORMANCE - 1, COMPARE_LESS);
    // Result.
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    printf("Time taken by qsort %d", ElapsedMicroseconds);
}

// Performance test.
TEST(test_performance, test_isort) {
    int *a = new int[N_PERFORMANCE];
    srand(time(NULL));
    for (int i = 0; i < N_PERFORMANCE; i += 1) {
        a[i] = rand() % N_PERFORMANCE;
    }

    // Init.
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    // Sort.
    insertion_sort(a, a + N_PERFORMANCE - 1, COMPARE_LESS);
    // Result.
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    printf("Time taken by isort %d", ElapsedMicroseconds);
}
