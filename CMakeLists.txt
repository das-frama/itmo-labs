cmake_minimum_required(VERSION 3.17)
project(itmo_labs)

set(CMAKE_CXX_STANDARD 14)

add_executable(itmo_labs_1 l1-sort/sort.h l1-sort/sort_test.cpp)
add_executable(itmo_labs_2 l2-containers/array_test.cpp l2-containers/array.h l2-containers/list_test.cpp l2-containers/list.h)
add_executable(itmo_labs_3 l3-search/tree.h l3-search/tree_test.cpp)
add_executable(itmo_labs_4 l4-allocation/memory.h l4-allocation/fsa.h l4-allocation/ca.h l4-allocation/abstract.h l4-allocation/page.h l4-allocation/allocators_test.cpp)

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
target_link_libraries(itmo_labs_1 gtest gtest_main)
target_link_libraries(itmo_labs_2 gtest gtest_main)
target_link_libraries(itmo_labs_3 gtest gtest_main)
target_link_libraries(itmo_labs_4 gtest gtest_main)

#add_subdirectory(l1-sort)
