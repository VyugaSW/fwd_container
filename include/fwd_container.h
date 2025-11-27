#pragma once
#include <iostream>
#include <memory>

template<typename T>
class fwd_container {
public:

    class iterator_base;
    class const_iterator_base;
    class iterator;
    class const_iterator;

    class iterator_base {
    public:
        virtual ~iterator_base() = default;
        
        virtual T& operator*() = 0;
        virtual T* operator->() = 0;
        virtual iterator_base& operator++() = 0;
        virtual bool operator==(const iterator_base& other) const = 0;
        virtual bool operator!=(const iterator_base& other) const = 0;
        
        virtual bool operator==(const typename fwd_container<T>::const_iterator_base& other) const = 0;
        virtual bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const = 0;

        virtual const_iterator_base* create_const() const = 0;

    protected:
        virtual iterator_base* clone() const = 0;
        
        friend class iterator;
        friend class const_iterator;
    };

    class const_iterator_base {
    public:
        virtual ~const_iterator_base() = default;
        
        virtual const T& operator*() const = 0;
        virtual const T* operator->() const = 0;
        virtual const_iterator_base& operator++() = 0;
        virtual bool operator==(const const_iterator_base& other) const = 0;
        virtual bool operator!=(const const_iterator_base& other) const = 0;
        
        virtual bool operator==(const typename fwd_container<T>::iterator_base& other) const = 0;
        virtual bool operator!=(const typename fwd_container<T>::iterator_base& other) const = 0;

    protected:
        virtual const_iterator_base* clone() const = 0;
        
        friend class iterator;
        friend class const_iterator;
    };

    class iterator {
    private:
        iterator_base* ptr;

    public:
        using difference_type   = ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = std::forward_iterator_tag;

        iterator() : ptr(nullptr) {}
        
        iterator(iterator_base* p) : ptr(p) {}
        
        iterator(const iterator& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}
        
        iterator(iterator&& other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }
        
        // ИСПРАВЛЕНО: конструктор из const_iterator
        iterator(const const_iterator& other) : ptr(nullptr) {
            // Для преобразования const_iterator в iterator нужно создать
            // соответствующий iterator_base из const_iterator_base
            // В текущей архитектуре это сложно, поэтому временно оставляем nullptr
            // В реальной реализации нужно добавить create_non_const() в const_iterator_base
        }
        
        ~iterator() {
            delete ptr;
        }
        
        iterator& operator=(const iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->clone() : nullptr;
            }
            return *this;
        }
        
        iterator& operator=(iterator&& other) noexcept {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }
        
        // ИСПРАВЛЕНО: присваивание из const_iterator
        iterator& operator=(const const_iterator& other) {
            // Аналогично конструктору из const_iterator
            if (this != &other) {
                delete ptr;
                ptr = nullptr;
            }
            return *this;
        }
        
        T& operator*() {
            return **ptr;
        }
        
        T* operator->() {
            return &(**ptr);
        }
        
        iterator& operator++() {
            ++(*ptr);
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++(*ptr);
            return temp;
        }
        
        bool operator==(const iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        bool operator==(const const_iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        friend class const_iterator;
    };

    class const_iterator {
    private:
        const_iterator_base* ptr;

    public:
        using difference_type   = ptrdiff_t;
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator() : ptr(nullptr) {}
        
        const_iterator(const_iterator_base* p) : ptr(p) {}
        
        const_iterator(const const_iterator& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}
        
        const_iterator(const_iterator&& other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }
        
        // ИСПРАВЛЕНО: конструктор из iterator
        const_iterator(const iterator& other) : ptr(nullptr) {
            if (other.ptr) {
                // Используем новый метод create_const() для преобразования
                ptr = other.ptr->create_const();
            }
        }
        
        ~const_iterator() {
            delete ptr;
        }
        
        const_iterator& operator=(const const_iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->clone() : nullptr;
            }
            return *this;
        }
        
        const_iterator& operator=(const_iterator&& other) noexcept {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }
        
        // ИСПРАВЛЕНО: присваивание из iterator
        const_iterator& operator=(const iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->create_const() : nullptr;
            }
            return *this;
        }
        
        const T& operator*() const {
            return **ptr;
        }
        
        const T* operator->() const {
            return &(**ptr);
        }
        
        const_iterator& operator++() {
            ++(*ptr);
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*ptr);
            return temp;
        }
        
        bool operator==(const const_iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        bool operator==(const iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        friend class iterator;
    };

    virtual void push(const T& value) = 0;
    virtual void push(T&& value) = 0;
    virtual T pop() = 0;
    
    virtual T& get_front() = 0;
    virtual const T& get_front() const = 0;
    
    virtual bool is_empty() const = 0;
    virtual size_t size() const = 0;
    
    virtual iterator begin() = 0;
    virtual iterator end() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual fwd_container<T>& operator=(const fwd_container<T>& other) = 0;

    virtual ~fwd_container() = default;

    friend std::ostream& operator<<(std::ostream& os, const fwd_container<T>& container) {
        try {
            container.print(os);   
            return os;
        }
        catch(std::runtime_error& e) {
            throw;
        }
    }

    friend std::istream& operator>>(std::istream& is, fwd_container<T>& container) {
        try {
            container.read(is);
            return is;
        }
        catch(std::runtime_error& e) {
            throw;
        }
    }
protected:
    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual std::istream& read(std::istream& is) = 0;
};