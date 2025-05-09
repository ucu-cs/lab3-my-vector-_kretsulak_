#include "testing_my_array.hpp"


void test_array_default_construction() {
    std::cout << "Running test_default_construction... ";
    my_array<int, 5> arr{};
    assert(arr.size() == 5);
    for (const auto& elem : arr) {
        assert(elem == int());
    }
    std::cout << "Passed!\n";
}

void test_array_initializer_list_construction() {
    std::cout << "Running test_initializer_list_construction... ";
    my_array<int, 5> arr = {1, 2, 3, 4, 5};
    assert(arr.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(arr[i] == i + 1);
    }
    std::cout << "Passed!\n";
}

void test_array_element_access() {
    std::cout << "Running test_element_access... ";
    my_array<std::string, 3> arr = {"first", "second", "third"};

    // Test operator[]
    assert(arr[0] == "first");
    assert(arr[1] == "second");
    assert(arr[2] == "third");

    // Test at()
    assert(arr.at(0) == "first");
    assert(arr.at(1) == "second");
    assert(arr.at(2) == "third");

    // Test front() and back()
    assert(arr.front() == "first");
    assert(arr.back() == "third");

    // Test data()
    assert(arr.data()[0] == "first");

    // Test exception for at()
    try {
        arr.at(3);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    std::cout << "Passed!\n";
}

void test_array_iterators() {
    std::cout << "Running test_iterators... ";
    my_array<int, 4> arr = {10, 20, 30, 40};

    // Test forward iteration
    int expected = 10;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        assert(*it == expected);
        expected += 10;
    }

    // Test reverse iteration
    expected = 40;
    for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
        assert(*it == expected);
        expected -= 10;
    }

    // Test const iteration
    const auto& carr = arr;
    expected = 10;
    for (auto it = carr.begin(); it != carr.end(); ++it) {
        assert(*it == expected);
        expected += 10;
    }
    std::cout << "Passed!\n";
}

void test_array_size_and_capacity() {
    std::cout << "Running test_size_and_capacity... ";
    my_array<double, 7> arr;

    assert(arr.size() == 7);
    assert(!arr.empty());
    assert(arr.max_size() == 7);
    std::cout << "Passed!\n";
}

void test_array_fill_operation() {
    std::cout << "Running test_fill_operation... ";
    my_array<char, 6> arr;
    arr.fill('x');

    for (const auto& c : arr) {
        assert(c == 'x');
    }
    std::cout << "Passed!\n";
}

void test_array_swap_operation() {
    std::cout << "Running test_swap_operation... ";
    my_array<int, 3> arr1 = {1, 2, 3};
    my_array<int, 3> arr2 = {4, 5, 6};

    arr1.swap(arr2);

    assert(arr1[0] == 4 && arr1[1] == 5 && arr1[2] == 6);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3);
    std::cout << "Passed!\n";
}

void test_array_comparison_operators() {
    std::cout << "Running test_comparison_operators... ";
    my_array<int, 3> arr1 = {1, 2, 3};
    my_array<int, 3> arr2 = {1, 2, 3};
    my_array<int, 3> arr3 = {1, 2, 4};
    my_array<int, 3> arr4 = {1, 1, 3};

    assert(arr1 == arr2);
    assert(arr1 != arr3);
    assert(arr1 < arr3);
    assert(arr3 > arr1);
    assert(arr4 < arr1);
    assert(arr1 <= arr2);
    assert(arr1 >= arr2);
    std::cout << "Passed!\n";
}

void test_array_complex_type() {
    std::cout << "Running test_complex_type... ";
    my_array<my_array<int, 2>, 3> arr = {
            my_array<int, 2>{1, 2},
            my_array<int, 2>{3, 4},
            my_array<int, 2>{5, 6}
    };

    assert(arr.size() == 3);
    assert(arr[0].size() == 2);
    assert(arr[0][0] == 1 && arr[0][1] == 2);
    assert(arr[1][0] == 3 && arr[1][1] == 4);
    assert(arr[2][0] == 5 && arr[2][1] == 6);
    std::cout << "Passed!\n";
}

void run_all_array_tests() {
    std::cout << "Starting all array tests...\n\n";

    test_array_default_construction();
    test_array_initializer_list_construction();
    test_array_element_access();
    test_array_iterators();
    test_array_size_and_capacity();
    test_array_fill_operation();
    test_array_swap_operation();
    test_array_comparison_operators();
    test_array_complex_type();

    std::cout << "\n\033[3;42;30m  All array tests passed successfully!  \033[0m" << std::endl;
}
