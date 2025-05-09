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
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;

private:
    pointer data_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;

    void reallocate(size_type new_capacity) {
        pointer new_data = nullptr;

        if (new_capacity > 0) {
            new_data = static_cast<pointer>(::operator new(new_capacity * sizeof(T)));

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

    my_vector(const my_vector& other) : my_vector(other.begin(), other.end()) {}

    my_vector(my_vector&& other) noexcept
            : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~my_vector() {
        destroy_elements();
        ::operator delete(data_);
    }

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

    bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return capacity_;
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }

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

        for (auto it = end() + count - 1; it != pos + count - 1; --it) {
            new (&*it) T(std::move(*(it - count)));
            (it - count)->~T();
        }

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

        for (auto it = end() + count - 1; it != pos + count - 1; --it) {
            new (&*it) T(std::move(*(it - count)));
            (it - count)->~T();
        }

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
