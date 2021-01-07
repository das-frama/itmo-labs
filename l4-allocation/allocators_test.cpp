//
// Created by Andrey Galaktionov on 29.12.2020.
//

#include "myfsa.h"
#include "gtest/gtest.h"

TEST(allocators, fsa_init) {
    FixedSizeAllocator a(16);
    a.alloc(16);
}
