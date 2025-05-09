#include "testing_my_vector.hpp"


void test_default_constructor() {
    std::cout << "Running test_default_constructor... ";
    my_vector<int> v;
    assert(v.empty());
    assert(v.size() == 0);
    std::cout << "Passed!\n";
}

void test_size_constructor() {
    std::cout << "Running test_size_constructor... ";
    my_vector<int> v(5);
    assert(v.size() == 5);
    for (auto x : v) {
        assert(x == 0);
    }
    std::cout << "Passed!\n";
}

void test_initializer_list() {
    std::cout << "Running test_initializer_list... ";
    my_vector<int> v = {1, 2, 3};
    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    std::cout << "Passed!\n";
}

void test_push_back() {
    std::cout << "Running test_push_back... ";
    my_vector<int> v;
    v.push_back(42);
    assert(v.size() == 1);
    assert(v[0] == 42);
    std::cout << "Passed!\n";
}

void test_emplace_back() {
    std::cout << "Running test_emplace_back... ";
    my_vector<std::string> v;
    v.emplace_back("test");
    assert(v.size() == 1);
    assert(v[0] == "test");
    std::cout << "Passed!\n";
}

void test_copy_constructor() {
    std::cout << "Running test_copy_constructor... ";
    my_vector<int> v1 = {1, 2, 3};
    my_vector<int> v2(v1);
    assert(v1.size() == v2.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        assert(v1[i] == v2[i]);
    }
    std::cout << "Passed!\n";
}

void test_move_constructor() {
    std::cout << "Running test_move_constructor... ";
    my_vector<int> v1 = {1, 2, 3};
    my_vector<int> v2(std::move(v1));
    assert(v2.size() == 3);
    assert(v1.empty());
    std::cout << "Passed!\n";
}

void test_self_assignment() {
    std::cout << "Running test_self_assignment... ";
    my_vector<int> v = {1, 2, 3};
    v = v;
    assert(v.size() == 3);
    assert(v[0] == 1);
    std::cout << "Passed!\n";
}

void test_iterator() {
    std::cout << "Running test_iterator... ";
    my_vector<int> v = {1, 2, 3};
    int sum = 0;
    for (auto x : v) {
        sum += x;
    }
    assert(sum == 6);
    std::cout << "Passed!\n";
}

void test_reserve() {
    std::cout << "Running test_reserve... ";
    my_vector<int> v;
    v.reserve(100);
    assert(v.capacity() >= 100);
    assert(v.empty());
    std::cout << "Passed!\n";
}

void test_insert() {
    std::cout << "Running test_insert... ";
    my_vector<int> v = {1, 3};
    v.insert(v.begin() + 1, 2);
    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    std::cout << "Passed!\n";
}

void test_erase() {
    std::cout << "Running test_erase... ";
    my_vector<int> v = {1, 2, 3, 4};
    v.erase(v.begin() + 1, v.begin() + 3);
    assert(v.size() == 2);
    assert(v[0] == 1);
    assert(v[1] == 4);
    std::cout << "Passed!\n";
}

void test_complex_type() {
    std::cout << "Running test_complex_type... ";
    my_vector<my_vector<int>> v;
    v.push_back({1, 2, 3});
    v.push_back({4, 5});
    assert(v.size() == 2);
    assert(v[0].size() == 3);
    assert(v[1].size() == 2);
    std::cout << "Passed!\n";
}

void run_all_tests() {
    std::cout << "Starting all tests...\n\n";

    test_default_constructor();
    test_size_constructor();
    test_initializer_list();
    test_push_back();
    test_emplace_back();
    test_copy_constructor();
    test_move_constructor();
    test_self_assignment();
    test_iterator();
    test_reserve();
    test_insert();
    test_erase();
    test_complex_type();

    std::cout << "\n\033[3;42;30m  All vector tests passed successfully!  \033[0m" << std::endl;
}
