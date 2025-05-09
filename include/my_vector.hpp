#ifndef MY_VECTOR_MY_VECTOR_HPP
#define MY_VECTOR_MY_VECTOR_HPP

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class my_vector {
public:
    // Типи
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

private:
    pointer data_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;

    void reallocate(size_type new_capacity) {
        pointer new_data = nullptr;

        if (new_capacity > 0) {
            new_data = static_cast<pointer>(::operator new(new_capacity * sizeof(T)));

            // Переміщення існуючих елементів
            for (size_type i = 0; i < size_; ++i) {
                new (&new_data[i]) T(std::move(data_[i]));
                data_[i].~T();
            }
        }

        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void destroy_elements() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

public:
    // Конструктори
    my_vector() noexcept = default;

    explicit my_vector(size_type count) {
        if (count > 0) {
            data_ = static_cast<pointer>(::operator new(count * sizeof(T)));
            capacity_ = count;
            try {
                for (size_ = 0; size_ < count; ++size_) {
                    new (&data_[size_]) T();
                }
            } catch (...) {
                destroy_elements();
                ::operator delete(data_);
                throw;
            }
        }
    }

    my_vector(size_type count, const T& value) {
        if (count > 0) {
            data_ = static_cast<pointer>(::operator new(count * sizeof(T)));
            capacity_ = count;
            try {
                for (size_ = 0; size_ < count; ++size_) {
                    new (&data_[size_]) T(value);
                }
            } catch (...) {
                destroy_elements();
                ::operator delete(data_);
                throw;
            }
        }
    }

    template <typename InputIt, typename = std::enable_if_t<std::is_base_of_v<
            std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
    my_vector(InputIt first, InputIt last) {
        size_type count = std::distance(first, last);
        if (count > 0) {
            data_ = static_cast<pointer>(::operator new(count * sizeof(T)));
            capacity_ = count;
            try {
                for (size_ = 0; first != last; ++first, ++size_) {
                    new (&data_[size_]) T(*first);
                }
            } catch (...) {
                destroy_elements();
                ::operator delete(data_);
                throw;
            }
        }
    }

    my_vector(std::initializer_list<T> init) : my_vector(init.begin(), init.end()) {}

    // Конструктори копіювання та переміщення
    my_vector(const my_vector& other) : my_vector(other.begin(), other.end()) {}

    my_vector(my_vector&& other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Деструктор
    ~my_vector() {
        destroy_elements();
        ::operator delete(data_);
    }

    // Оператори присвоєння
    my_vector& operator=(const my_vector& other) {
        if (this != &other) {
            my_vector temp(other);
            swap(temp);
        }
        return *this;
    }

    my_vector& operator=(my_vector&& other) noexcept {
        if (this != &other) {
            destroy_elements();
            ::operator delete(data_);

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    my_vector& operator=(std::initializer_list<T> init) {
        my_vector temp(init);
        swap(temp);
        return *this;
    }

    // Доступ до елементів
    reference operator[](size_type pos) noexcept {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const noexcept {
        return data_[pos];
    }

    reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("my_vector::at");
        }
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("my_vector::at");
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
        return data_[size_ - 1];
    }

    const_reference back() const noexcept {
        return data_[size_ - 1];
    }

    pointer data() noexcept {
        return data_;
    }

    const_pointer data() const noexcept {
        return data_;
    }

    // Ітератори
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
        return data_ + size_;
    }

    const_iterator end() const noexcept {
        return data_ + size_;
    }

    const_iterator cend() const noexcept {
        return data_ + size_;
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

    // Ємність
    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }

    // Модифікатори
    void clear() noexcept {
        destroy_elements();
    }

    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) return const_cast<iterator>(pos);

        size_type index = pos - begin();
        if (size_ + count > capacity_) {
            size_type new_capacity = std::max(size_ + count, capacity_ * 2);
            reserve(new_capacity);
        }

        pos = begin() + index;

        // Зсув елементів вправо
        for (auto it = end() + count - 1; it != pos + count - 1; --it) {
            new (&*it) T(std::move(*(it - count)));
            (it - count)->~T();
        }

        // Вставка нових елементів
        for (size_type i = 0; i < count; ++i) {
            new (&*(pos + i)) T(value);
        }

        size_ += count;
        return begin() + index;
    }

    template <typename InputIt, typename = std::enable_if_t<std::is_base_of_v<
            std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type count = std::distance(first, last);
        if (count == 0) return const_cast<iterator>(pos);

        size_type index = pos - begin();
        if (size_ + count > capacity_) {
            size_type new_capacity = std::max(size_ + count, capacity_ * 2);
            reserve(new_capacity);
        }

        pos = begin() + index;

        // Зсув елементів вправо
        for (auto it = end() + count - 1; it != pos + count - 1; --it) {
            new (&*it) T(std::move(*(it - count)));
            (it - count)->~T();
        }

        // Вставка нових елементів
        for (size_type i = 0; i < count; ++i, ++first) {
            new (&*(pos + i)) T(*first);
        }

        size_ += count;
        return begin() + index;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> init) {
        return insert(pos, init.begin(), init.end());
    }

    template <typename... Args>
    iterator emplace(const_iterator cpos, Args&&... args) {
        size_type index = cpos - begin();
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        iterator pos = begin() + index;

        if (pos != end()) {
            // Move elements to the right
            new (&*end()) T(std::move(back()));
            back().~T();
            for (auto it = end() - 1; it != pos; --it) {
                *it = std::move(*(it - 1));
            }
            *pos = T(std::forward<Args>(args)...);
        } else {
            new (&*pos) T(std::forward<Args>(args)...);
        }

        ++size_;
        return begin() + index;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        if (first == last) return const_cast<iterator>(first);

        size_type count = last - first;
        size_type index = first - begin();

        for (auto it = const_cast<iterator>(first); it != end() - count; ++it) {
            *it = std::move(*(it + count));
        }

        for (auto it = end() - count; it != end(); ++it) {
            it->~T();
        }

        size_ -= count;
        return begin() + index;
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        new (&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
        return back();
    }

    void pop_back() noexcept {
        if (size_ > 0) {
            --size_;
            data_[size_].~T();
        }
    }

    void resize(size_type count) {
        if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i) {
                new (&data_[i]) T();
            }
        } else if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = count;
    }

    void resize(size_type count, const T& value) {
        if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i) {
                new (&data_[i]) T(value);
            }
        } else if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = count;
    }

    void swap(my_vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Оператори порівняння
    bool operator==(const my_vector& other) const {
        if (size_ != other.size_) return false;
        return std::equal(begin(), end(), other.begin());
    }

    bool operator!=(const my_vector& other) const {
        return !(*this == other);
    }

    bool operator<(const my_vector& other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }

    bool operator<=(const my_vector& other) const {
        return !(other < *this);
    }

    bool operator>(const my_vector& other) const {
        return other < *this;
    }

    bool operator>=(const my_vector& other) const {
        return !(*this < other);
    }
};

template <typename T>
void swap(my_vector<T>& lhs, my_vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

#endif // MY_VECTOR_MY_VECTOR_HPP




//#ifndef MY_VECTOR_MY_VECTOR_HPP
//#define MY_VECTOR_MY_VECTOR_HPP
//
//#include <algorithm>
//#include <stdexcept>
//#include <initializer_list>
//#include <iterator>
//#include <memory>
//#include <iostream>
//
//template<typename T>
//class my_vector {
//private:
//    T* data;
//    std::size_t size_;
//    std::size_t capacity_;
//
//    void reallocate(std::size_t new_capacity) {
//        T* new_data = new T[new_capacity];
//        std::move(data, data + size_, new_data);
//        delete[] data;
//        data = new_data;
//        capacity_ = new_capacity;
//    }
//
//public:
//    using iterator = T*;                // Define an iterator type as a pointer to T
//    using const_iterator = const T*;    // Define const_iterator type as a pointer to const T
//
//    // Constructors
//    my_vector() : data(nullptr), size_(0), capacity_(0) {}
//
//    explicit my_vector(std::size_t n, const T& value = T())
//            : data(new T[n]), size_(n), capacity_(n) {
//        std::fill(data, data + n, value);
//    }
//
//    my_vector(std::initializer_list<T> init)
//            : data(new T[init.size()]), size_(init.size()), capacity_(init.size()) {
//        std::copy(init.begin(), init.end(), data);
//    }
//
//    my_vector(const my_vector& other)
//            : data(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
//        std::copy(other.data, other.data + other.size_, data);
//    }
//
//    my_vector(my_vector&& other) noexcept
//            : data(other.data), size_(other.size_), capacity_(other.capacity_) {
//        other.data = nullptr;
//        other.size_ = 0;
//        other.capacity_ = 0;
//    }
//
//    // Destructor
//    ~my_vector() {
//        delete[] data;
//    }
//
//    // Assignment operators
//    my_vector& operator=(const my_vector& other) {
//        if (this != &other) {
//            T* new_data = new T[other.capacity_];
//            std::copy(other.data, other.data + other.size_, new_data);
//            delete[] data;
//            data = new_data;
//            size_ = other.size_;
//            capacity_ = other.capacity_;
//        }
//        return *this;
//    }
//
//    my_vector& operator=(my_vector&& other) noexcept {
//        if (this != &other) {
//            delete[] data;
//            data = other.data;
//            size_ = other.size_;
//            capacity_ = other.capacity_;
//            other.data = nullptr;
//            other.size_ = 0;
//            other.capacity_ = 0;
//        }
//        return *this;
//    }
//
//    // Element access
//    T& operator[](std::size_t index) {
//        return data[index];
//    }
//
//    const T& operator[](std::size_t index) const {
//        return data[index];
//    }
//
//    T& at(std::size_t index) {
//        if (index >= size_) {
//            throw std::out_of_range("Index out of range");
//        }
//        return data[index];
//    }
//
//    const T& at(std::size_t index) const {
//        if (index >= size_) {
//            throw std::out_of_range("Index out of range");
//        }
//        return data[index];
//    }
//
//    T& front() {
//        return data[0];
//    }
//
//    const T& front() const {
//        return data[0];
//    }
//
//    T& back() {
//        return data[size_ - 1];
//    }
//
//    const T& back() const {
//        return data[size_ - 1];
//    }
//
//    // Iterators
//    T* begin() {
//        return data;
//    }
//
//    const T* begin() const {
//        return data;
//    }
//
//    T* end() {
//        return data + size_;
//    }
//
//    const T* end() const {
//        return data + size_;
//    }
//
//    // Size and capacity
//    [[nodiscard]] std::size_t size() const {
//        return size_;
//    }
//
//    [[nodiscard]] std::size_t capacity() const {
//        return capacity_;
//    }
//
//    [[nodiscard]] bool is_empty() const {
//        return size_ == 0;
//    }
//
//    void reserve(std::size_t new_capacity) {
//        if (new_capacity > capacity_) {
//            reallocate(new_capacity);
//        }
//    }
//
//    void shrink_to_fit() {
//        if (size_ < capacity_) {
//            reallocate(size_);
//        }
//    }
//
//    // Modifiers
//    void clear() {
//        size_ = 0;
//    }
//
//    void resize(std::size_t new_size, const T& value = T()) {
//        if (new_size > size_) {
//            if (new_size > capacity_) {
//                reserve(new_size);
//            }
//            std::fill(data + size_, data + new_size, value);
//        }
//        size_ = new_size;
//    }
//
//    void push_back(const T& value) {
//        if (size_ == capacity_) {
//            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
//        }
//        data[size_] = value;
//        ++size_;
//    }
//
//    void push_back(T&& value) {
//        if (size_ == capacity_) {
//            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
//        }
//        data[size_] = std::move(value);
//        ++size_;
//    }
//
//    void pop_back() {
//        if (size_ > 0) {
//            --size_;
//        }
//    }
//
//    // Insert
//    void insert(iterator position, const T& value) {
//        if (position < begin() || position > end()) {
//            throw std::out_of_range("Iterator out of range");
//        }
//        if (size_ == capacity_) {
//            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
//        }
//
//        std::size_t index = position - begin();  // Store position as index before realloc
//        position = begin() + index;
//        ++size_; // Increase size before moving elements to make end() valid
//
//        // Shift elements to the right
//        for (iterator it = end() - 1; it > position; --it) {
//            *it = *(it - 1);
//        }
//
//        *position = value;
//
////        for (iterator it = end() - 1; it != begin() + index; --it) {
////            *it = *(it - 1);
////        }
////
////        data[index] = value;
//    }
//
////    void insert(iterator position, const T& value) {
////        if (position < begin() || position > end()) {
////            throw std::out_of_range("Iterator out of range");
////        }
////
////        std::size_t index = position - begin(); // Save index before any possible reallocation
////
////        if (size_ == capacity_) {
////            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
////        }
////
////        // Increase size BEFORE using end()
////        ++size_;
////
////        // Recalculate position after reallocation
////        position = begin() + index;
////
////        // Shift elements to the right
////        for (iterator it = end() - 1; it > position; --it) {
////            *it = *(it - 1);
////        }
////
////        *position = value;
////    }
//
//    // Insert a range of elements
//    template <typename InputIterator>
//    void insert(iterator position, InputIterator first, InputIterator last) {
//        if (position < begin() || position > end()) {
//            throw std::out_of_range("Iterator out of range");
//        }
//
//        // Calculate the number of elements to insert
//        auto count = std::distance(first, last);
//
//        if (size_ + count > capacity_) {
//            reserve(size_ + count);
//        }
//
//        // Shift elements to the right
//        for (iterator it = end() + count - 1; it >= position + count; --it) {
//            *it = *(it - count);
//        }
//
//        // Insert the range
//        for (iterator it = position; first != last; ++it, ++first) {
//            *it = *first;
//        }
//
//        size_ += count;
//    }
//
//    // Erase
//    void erase(iterator position) {
//        if (position < begin() || position >= end()) {
//            throw std::out_of_range("Iterator out of range");
//        }
//        for (iterator it = position; it != end() - 1; ++it) {
//            *it = *(it + 1);
//        }
//        --size_;
//    }
//
//    // Swap
//    void swap(my_vector& other) {
//        std::swap(data, other.data);
//        std::swap(size_, other.size_);
//        std::swap(capacity_, other.capacity_);
//    }
//};
//
//// Comparison operators
//template<typename T>
//bool operator==(const my_vector<T>& lhs, const my_vector<T>& rhs) {
//    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
//}
//
//template<typename T>
//bool operator!=(const my_vector<T>& lhs, const my_vector<T>& rhs) {
//    return !(lhs == rhs);
//}
//
//template<typename T>
//bool operator<(const my_vector<T>& lhs, const my_vector<T>& rhs) {
//    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
//}
//
//// For std::back_inserter to work
//template<typename T>
//void push_back(my_vector<T>& vec, const T& value) {
//    vec.push_back(value);
//}
//
//template<typename T>
//void push_back(my_vector<T>& vec, T&& value) {
//    vec.push_back(std::move(value));
//}
//
//#endif //MY_VECTOR_MY_VECTOR_HPP
