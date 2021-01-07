//
// Created by Andrey Galaktionov on 29.12.2020.
//

#ifndef ITMO_LABS_FSA_H
#define ITMO_LABS_FSA_H

#include <cassert>
#include "windows.h"
#include "abstract.h"

class FixedSizeAllocator : public AbstractAllocator {
public:
    explicit FixedSizeAllocator(size_t size) : _blockSize(size) {
        assert(size >= sizeof(Block));
    }

    ~FixedSizeAllocator() override {
        assert(_memory == nullptr);

        if (_memory != nullptr) {
            destroy();
        }
    }

    void init() override {
        assert(_memory == nullptr);

        createPage();
    }

    void destroy() final {
        assert(_memory != nullptr);

        MemPage *tmp;
        do {
            tmp = _memory->nextPage;
            VirtualFree(_memory, PAGE_SIZE, MEM_RELEASE);
            _memory = tmp;
        } while (_memory != nullptr);
    }

    void *alloc(size_t size) override {
        assert(_memory != nullptr);
        assert(size <= _blockSize);

        if (_freeBlocksHead == nullptr) {
            createPage();
        }

        size_t allBlocks = DATA_SIZE / _blockSize;
        if (_initializedBlocks < allBlocks) {
            Block *block = getBlock(_initializedBlocks);
            block->next = ++_initializedBlocks < allBlocks ? getBlock(_initializedBlocks) : nullptr;
        }

        auto block = takeBlock();
        return block;
    }

    void free(void *p) override {
        assert(_memory != nullptr);

        auto block = reinterpret_cast<Block *>(p);
        block->next = _freeBlocksHead;
        _freeBlocksHead = block;
    }

    bool isInAllocRange(void *p) const override {
        for (MemPage *page = _memory; page != nullptr; page = page->nextPage) {
            bool inPage = (u8*) (page) < (u8*)(p) && (u8*)(p) < (u8*)(page) + PAGE_SIZE;
            if (inPage) {
                bool correctPadding = ((u8*)(p) - (u8*)(page + 1)) % _blockSize == 0;
                return  correctPadding;
            }
        }
        return false;
    }

    size_t maxAllocSize() const override {
        return _blockSize;
    }

private:
    struct Block {
        Block *next;
    };
    struct MemPage {
        MemPage *nextPage;
    };

private:
    static const size_t DATA_SIZE = 1 << 23;
    static const size_t PAGE_SIZE = 4096;

    const size_t _blockSize;
    MemPage *_memory = nullptr;
    Block *_freeBlocksHead = nullptr;
    size_t _initializedBlocks = 0;

    Block *getBlock(int i, MemPage *page) const {
        return (Block *) ((u8 *) (page + 1) + i * _blockSize);
    }

    Block *getBlock(int i) const {
        return getBlock(i, _memory);
    }

    Block *firstBlock() const {
        return (Block *) (_memory + 1);
    }

    void createPage() {
        MemPage *oldPage = _memory;
        _memory = reinterpret_cast<MemPage *>(VirtualAlloc(nullptr, PAGE_SIZE, MEM_COMMIT, PAGE_READWRITE));
        _memory->nextPage = oldPage;
        _freeBlocksHead = firstBlock();
        _initializedBlocks = 0;
    }

    Block *takeBlock() {
        Block *block = _freeBlocksHead;
        _freeBlocksHead = _freeBlocksHead->next;
        return block;
    }
};

#endif //ITMO_LABS_FSA_H
