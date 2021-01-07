//
// Created by Andrey Galaktionov on 29.12.2020.
//

#ifndef ITMO_LABS_ABSTRACT_H
#define ITMO_LABS_ABSTRACT_H

#include <cstdint>

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

typedef int b32;

class AbstractAllocator {

    virtual void init() = 0;

    virtual void destroy() = 0;

    virtual void *alloc(size_t size) = 0;

    virtual void free(void *p) = 0;

//    virtual bool isInAllocRange(void *p) const = 0;
////
//    virtual size_t maxAllocSize() const = 0;

#ifdef DEBUG
    virtual void dumpStat() const = 0;

    virtual void dumpBlock() const = 0;
#endif
protected:
    virtual ~AbstractAllocator() = default;
};

#endif //ITMO_LABS_ABSTRACT_H
