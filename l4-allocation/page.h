//
// Created by Andrey Galaktionov on 29.12.2020.
//

#ifndef ITMO_LABS_PAGE_H
#define ITMO_LABS_PAGE_H

#include <windows.h>

class PageAllocator : public AbstractAllocator {
    void init() override {}

    void destroy() override {}

    void *alloc(size_t size) override {
        size = size + sizeof(size_t);
        size_t *p = (size_t) VirtualAlloc(nullptr, size, MEM_COMMIT, PAGE_READWRITE);
        p[0] = size;
        return p + 1;
    }

    void free(void *p) override {
        size_t *sp = reinterpret_cast<size_t *>(p) - 1;
        VirtualFree(p, size, MEM_RELEASE);
    }

    bool isInAllocRange(void *p) const override {
        return true;
    }

    size_t maxAllocSize() const override {
        return (size_t) (1) << (sizeof(size_t) * 8 - 1);
    }
};

#endif //ITMO_LABS_PAGE_H
