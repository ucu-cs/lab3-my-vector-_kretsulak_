#ifndef MY_VECTOR_TESTING_MY_VECTOR_HPP
#define MY_VECTOR_TESTING_MY_VECTOR_HPP

#include <iostream>
#include <cassert>
#include "my_vector.hpp"


void test_default_constructor();
void test_size_constructor();
void test_initializer_list();
void test_push_back();
void test_emplace_back();
void test_copy_constructor();
void test_move_constructor();
void test_self_assignment();
void test_iterator();
void test_reserve();
void test_insert();
void test_erase();
void test_complex_type();

void run_all_tests();

#endif //MY_VECTOR_TESTING_MY_VECTOR_HPP