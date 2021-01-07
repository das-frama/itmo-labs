//
// Created by Andrey Galaktionov on 20.12.2020.
//

#ifndef ITMO_LABS_MEMORY_H
#define ITMO_LABS_MEMORY_H

#include <cassert>
#include "fsa.h"
#include "ca.h"
#include "page.h"

class MemoryAllocator : public AbstractAllocator {
    static const size_t N = 8;

    FixedSizeAllocator fsa16;
    FixedSizeAllocator fsa32;
    FixedSizeAllocator fsa64;
    FixedSizeAllocator fsa128;
    FixedSizeAllocator fsa256;
    FixedSizeAllocator fsa512;
    CoalesceAllocator ca;
    PageAllocator ca;

    AbstractAllocator **allocators;

    bool initialized = false;

public:
    MemoryAllocator() : fsa16(16), fsa32(32), fsa64(64), fsa128(128),
                        fsa256(256), fsa512(512), ca(), pa() {
        allocators = new AbstractAllocator *[N]{&fsa16, &fsa32, &fsa64, &fsa128, &fsa256, &fsa512, &ca, &pa};
    }

    ~MemoryAllocator() override {
        assert(!initialized);
        if (initialized) {
            destroy();
        }

        // delete allocators.
    }

    // Init all allocators.
    void init() override {
        for (int i = 0; i < N; i += 1) {
            allocators[i]->init();
        }
        initialized = true;
    }

    // Destroy all allocators.
    void destroy() override {
        for (int i = 0; i < N; i += 1) {
            allocators[i]->destroy();
        }
        initialized = false;
    }

    void *alloc(size_t size) override {
        for (int i = 0; i < N; i += 1) {
            if (size < allocators[i]->maxAllocSize()) {
                return allocators[i]->allo c(size);
            }
        }
        return nullptr;
    }

    void free(void *p) override {
        for (int i = 0; i < N; i += 1) {
            if (allocators[i]->isInAllocRange(p)) {
                allocators[i]->free(p);
                return;
            }
        }
    }

    size_t maxAllocSize() const override {
        return pa.maxAllocSize();
    }
};

#endif //ITMO_LABS_MEMORY_H
