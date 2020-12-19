//
// Created by Andrey Galaktionov on 19.12.2020.
//

#ifndef ITMO_LABS_SORT_H
#define ITMO_LABS_SORT_H

// Simple swap.
template<typename T>
void swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template<typename T, typename Compare>
void insertion_sort(T* first, T* last, Compare comp) {
    T key;
    for (T* i = first + 1, *j = 0; i <= last; i += 1) {
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
void sort3(T* a, T* b, T* c, Compare comp) {
    if (comp(*b, *a)) swap(a, b);
    if (comp(*c, *a)) swap(a, c);
    if (comp(*c, *b)) swap(b, c);
}

// Quicksort with some optimisations.
template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    if (first >= last) {
        return;
    }

    T* i = first;
    T* j = last;
    T* p = first + (last - first) / 2;

    sort3(i, j, p, comp);

    while (i < j) {
        while (comp(*i, *p) && i < last) i += 1;
        while (comp(*p, *j) && !comp(*j, *p)) j -= 1;
        if (i < j) {
            swap(i, j);
        }
    }

    swap(p, j);

    sort(first, j - 1, comp);
    sort(j + 1, last, comp);
}

#endif //ITMO_LABS_SORT_H
