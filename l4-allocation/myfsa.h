//
// Created by Andrey Galaktionov on 04.01.2021.
//

#ifndef ITMO_LABS_MYFSA_H
#define ITMO_LABS_MYFSA_H

#include <cassert>
#include <windows.h>
#include "abstract.h"

class FixedSizeAllocator : public AbstractAllocator {
public:
    explicit FixedSizeAllocator(size_t size) : _blockSize(size), _freeHead(0) {
        assert(size >= sizeof(Block));
        _allBlocks = BUFFER_SIZE / size;
    }

    void init() override {

    }

    void destroy() override {

    }

    void *alloc(size_t size) override {
        if (_freeHead == -1) {
            _newPage();
        }
//
//        if (_initBlocks < _allBlocks) {
//            //
//        } else {
//            _newPage();
//        }
        return nullptr;
    }

    void free(void *p) override {
        *((int *) p) = _freeHead;
        _freeHead = (int) (((u8 *) p - (u8 *) _memory) / _blockSize);
    }

private:
    struct Block {
        Block *next;
    };

    struct Page {
        Page *next;
    };

private:
    // ca - 02:25:09
    void _newPage() {
        _memory = (Page *) (VirtualAlloc(nullptr, BUFFER_SIZE, MEM_COMMIT, PAGE_READWRITE));
        _freeHead = 0;
        _initBlocks = 0;
    }

    Block *_initBlock(int i) {
        return (Block *) ((u8 *) _memory + 1 + i * _blockSize);
    }

private:
    static const size_t BUFFER_SIZE = 4096;

    const size_t _blockSize;
    void *_memory = nullptr;
//    Page *_bufferMemory = nullptr;
    int _freeHead = -1;
    size_t _initBlocks = 0;
    size_t _allBlocks = 0;

};

#endif //ITMO_LABS_MYFSA_H
