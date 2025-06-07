#ifndef VEKTOR_H
#define VEKTOR_H
#include<string> 
#include<algorithm>

template <typename T>
class Vector {
public:
    class ConstIterator;
    class Iterator;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
    static constexpr size_type min_capacity = 5;
    size_type sz;
    size_type max_sz;
    pointer values;

    void allocate(size_type new_cap) {
        if (new_cap < sz) return;
        pointer new_vals = new value_type[new_cap];
        std::copy(values, values + sz, new_vals); 
        delete[] values;
        values = new_vals;
        max_sz = new_cap;
    }

public:
    Vector() : Vector(0) {}

    Vector(size_type n) : sz{0}, max_sz{n < min_capacity ? min_capacity : n}, values{new value_type[max_sz]} {}

    Vector(std::initializer_list<value_type> il) : Vector(il.size()) {
        for (const auto& val : il) {
            values[sz++] = val;
        }
    }

    Vector(const Vector& src) : Vector(src.max_sz) {
        for (size_type i = 0; i < src.sz; ++i) {
            values[i] = src.values[i];
        }
        sz = src.sz;
    }

    Vector& operator=(Vector copy) {
        std::swap(sz, copy.sz);
        std::swap(max_sz, copy.max_sz);
        std::swap(values, copy.values);
        return *this;
    }

    ~Vector() {
        delete[] values;
    }

    size_type size() const { 
        return sz; 
    }

    size_type capacity() const { 
        return max_sz; 
    }

    bool empty() const { 
        return sz == 0; 
    }

    void pop_back() {
        if (empty()) throw std::runtime_error("Empty Vector");
        --sz;
    }

    void clear() {
        delete[] values;
        values = new value_type[max_sz];
        sz = 0;
    }

    void push_back(const value_type& val) {
        if (sz >= max_sz) allocate(max_sz * 2);
        values[sz++] = val;
    }

    void reserve(size_type n) {
        if (n > max_sz) allocate(n);
    }

    void shrink_to_fit() {
        allocate(sz);
    }

    reference operator[](size_type index) {
        if (index >= sz) throw std::out_of_range("Index out of range");
        return values[index];
    }

    const_reference operator[](size_type index) const {
        if (index >= sz) throw std::out_of_range("Index out of range");
        return values[index];
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
        os << "[";
        for (size_type i = 0; i < v.sz; ++i) {
            if (i > 0) os << ", ";
            os << v[i];
        }
        os << "]";
        return os;
    }

    iterator begin() { 
        return iterator(values); 
    }

    iterator end() { 
        return iterator(values + sz); 
    }

    const_iterator begin() const { 
        return const_iterator(values); 
    }

    const_iterator end() const { 
        return const_iterator(values + sz); 
    }

    class ConstIterator {
    public:
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;

    public:
        ConstIterator() : ptr{nullptr} {}
        ConstIterator(pointer p) : ptr{p} {}

        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }

        bool operator==(const ConstIterator& other) const { 
            return ptr == other.ptr; 
        }

        bool operator!=(const ConstIterator& other) const { 
            return ptr != other.ptr; 
        }

        ConstIterator& operator++() {
            ++ptr;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++ptr;
            return temp;
        }

        friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr - rhs.ptr;
        }
    };

    class Iterator {
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

    private:
        pointer ptr;

    public:
        Iterator() : ptr{nullptr} {}
        Iterator(pointer p) : ptr{p} {}

        reference operator*() const { 
            return *ptr; 
        }

        pointer operator->() const { 
            return ptr; 
        }

        bool operator==(const Iterator& other) const { 
            return ptr == other.ptr; 
        }

        bool operator!=(const Iterator& other) const { 
            return ptr != other.ptr; 
        }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr;
            return temp;
        }

        operator ConstIterator() const {
            return ConstIterator(ptr);
        }
    };

    iterator insert(const_iterator pos, const_reference val) {
        difference_type diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) > sz)
            throw std::out_of_range("Insert position out of bounds");

        if (sz >= max_sz) allocate(max_sz * 2);
        for (size_type i = sz; i > static_cast<size_type>(diff); --i) {
            values[i] = values[i - 1];
        }
        values[diff] = val;
        ++sz;
        return iterator(values + diff);
    }

    iterator erase(const_iterator pos) {
        difference_type diff = pos - begin();
        if (diff < 0 || static_cast<size_type>(diff) >= sz)
            throw std::out_of_range("Erase position out of bounds");

        for (size_type i = static_cast<size_type>(diff); i < sz - 1; ++i) {
            values[i] = values[i + 1];
        }
        --sz;
        return iterator(values + diff);
    }

};

#endif
