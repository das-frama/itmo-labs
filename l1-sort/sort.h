//
// Created by Andrey Galaktionov on 19.12.2020.
//

#ifndef ITMO_LABS_SORT_H
#define ITMO_LABS_SORT_H

#define INSERTION_SORT_SHOULD_USE 150

// Simple swap.
template<typename T>
void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

// Insertion sort.
template<typename T, typename Compare>
void insertion_sort(T *first, T *last, Compare comp) {
    T key;
    for (T *i = first + 1, *j = 0; i <= last; i += 1) {
        key = *i;
        j = i - 1;
        while (j >= first && comp(key, *j)) {
            *(j + 1) = *j;
            j -= 1;
        }
        *(j + 1) = key;
    }
}

// Sort between 3 elements (median).
template<typename T, typename Compare>
void sort3(T *a, T *b, T *c, Compare comp) {
    if (comp(*b, *a)) swap(a, b);
    if (comp(*c, *a)) swap(a, c);
    if (comp(*c, *b)) swap(b, c);
}

// Partition.
template<typename T, typename Compare>
T *partition(T *first, T *last, const T p, Compare comp) {
    auto i = first, j = last;
    while (true) {
        while (comp(*i, p)) i += 1;
        while (comp(p, *j)) j -= 1;
        if (i >= j) {
            return j;
        }
        swap(i, j);
        i += 1;
        j -= 1;
    }
}

// Quicksort with some optimisations.
template<typename T, typename Compare>
void sort(T *first, T *last, Compare comp) {
    while (first < last) {
        // Decide if we should use insertion sort.
//        int n = last - first;
//        if (n <= INSERTION_SORT_SHOULD_USE) {
//            return insertion_sort(first, last, comp);
//        }

        // Sort between first, middle and last (median).
        auto p = first + (last - first) / 2;
        sort3(first, p, last, comp);

        // Sort.
        p = partition(first, last, *p, comp);
        if (p - first < last - p) {
            sort(first, p, comp);
            first = p + 1;
        } else {
            sort(p + 1, last, comp);
            last = p;
        }
    }
}

#endif //ITMO_LABS_SORT_H
