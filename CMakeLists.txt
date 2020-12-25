cmake_minimum_required(VERSION 3.17)
project(itmo_labs)

set(CMAKE_CXX_STANDARD 14)

add_executable(itmo_labs_1 l1-sort/sort.h l1-sort/sort_test.cpp)
add_executable(itmo_labs_2 l2-containers/array_test.cpp l2-containers/array.h)

add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
target_link_libraries(itmo_labs_1 gtest gtest_main)
target_link_libraries(itmo_labs_2 gtest gtest_main)

#add_subdirectory(l1-sort)
