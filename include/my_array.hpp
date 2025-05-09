//
// Created by Ksusha Kretsula on 09.05.2025.
//

#ifndef MY_VECTOR_MY_ARRAY_HPP
#define MY_VECTOR_MY_ARRAY_HPP

// my_array.h
#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T, std::size_t N>
class my_array {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    my_array() = default;

    my_array(std::initializer_list<T> init) {
        if (init.size() > N) {
            throw std::out_of_range("Too many elements in initializer list");
        }
        std::copy(init.begin(), init.end(), begin());
        if (init.size() < N) {
            std::fill(begin() + init.size(), end(), T());
        }
    }

    reference operator[](size_type pos) noexcept {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const noexcept {
        return data_[pos];
    }

    reference at(size_type pos) {
        if (pos >= N) {
            throw std::out_of_range("my_array::at");
        }
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= N) {
            throw std::out_of_range("my_array::at");
        }
        return data_[pos];
    }

    reference front() noexcept {
        return data_[0];
    }

    const_reference front() const noexcept {
        return data_[0];
    }

    reference back() noexcept {
        return data_[N - 1];
    }

    const_reference back() const noexcept {
        return data_[N - 1];
    }

    pointer data() noexcept {
        return data_;
    }

    const_pointer data() const noexcept {
        return data_;
    }

    iterator begin() noexcept {
        return data_;
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    const_iterator cbegin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return data_ + N;
    }

    const_iterator end() const noexcept {
        return data_ + N;
    }

    const_iterator cend() const noexcept {
        return data_ + N;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    bool empty() const noexcept {
        return N == 0;
    }

    size_type size() const noexcept {
        return N;
    }

    size_type max_size() const noexcept {
        return N;
    }

    void fill(const T& value) {
        std::fill(begin(), end(), value);
    }

    void swap(my_array& other) noexcept(std::is_nothrow_swappable_v<T>) {
        std::swap_ranges(begin(), end(), other.begin());
    }

    bool operator==(const my_array& other) const {
        return std::equal(begin(), end(), other.begin());
    }

    bool operator!=(const my_array& other) const {
        return !(*this == other);
    }

    bool operator<(const my_array& other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }

    bool operator<=(const my_array& other) const {
        return !(other < *this);
    }

    bool operator>(const my_array& other) const {
        return other < *this;
    }

    bool operator>=(const my_array& other) const {
        return !(*this < other);
    }

private:
    T data_[N];
};

template <typename T, std::size_t N>
void swap(my_array<T, N>& lhs, my_array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

#endif // MY_ARRAY_H

#endif //MY_VECTOR_MY_ARRAY_HPP
