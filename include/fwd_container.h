#pragma once
#include <iostream>
#include <memory>

/**
 * @brief A forward container interface with iterator support
 * @tparam T The type of elements stored in the container
 * 
 * This abstract class defines the interface for forward containers
 * that support iteration and basic container operations.
 */
template<typename T>
class fwd_container {
public:

    class iterator_base;
    class const_iterator_base;
    class iterator;
    class const_iterator;

    /**
     * @brief Base abstract iterator class for non-const iteration
     */
    class iterator_base {
    public:
        virtual ~iterator_base() = default;
        
        /**
         * @brief Dereference operator
         * @return Reference to the current element
         */
        virtual T& operator*() = 0;
        
        /**
         * @brief Member access operator
         * @return Pointer to the current element
         */
        virtual T* operator->() = 0;
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        virtual iterator_base& operator++() = 0;
        
        /**
         * @brief Equality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are equal
         */
        virtual bool operator==(const iterator_base& other) const = 0;
        
        /**
         * @brief Inequality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are not equal
         */
        virtual bool operator!=(const iterator_base& other) const = 0;
        
        /**
         * @brief Equality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are equal
         */
        virtual bool operator==(const typename fwd_container<T>::const_iterator_base& other) const = 0;
        
        /**
         * @brief Inequality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are not equal
         */
        virtual bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const = 0;

        /**
         * @brief Create a const version of this iterator
         * @return Pointer to new const_iterator_base
         */
        virtual const_iterator_base* create_const() const = 0;

    protected:
        /**
         * @brief Clone this iterator
         * @return Pointer to new cloned iterator_base
         */
        virtual iterator_base* clone() const = 0;
        
        friend class iterator;
        friend class const_iterator;
    };

    /**
     * @brief Base abstract iterator class for const iteration
     */
    class const_iterator_base {
    public:
        virtual ~const_iterator_base() = default;
        
        /**
         * @brief Dereference operator
         * @return Const reference to the current element
         */
        virtual const T& operator*() const = 0;
        
        /**
         * @brief Member access operator
         * @return Const pointer to the current element
         */
        virtual const T* operator->() const = 0;
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        virtual const_iterator_base& operator++() = 0;
        
        /**
         * @brief Equality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are equal
         */
        virtual bool operator==(const const_iterator_base& other) const = 0;
        
        /**
         * @brief Inequality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are not equal
         */
        virtual bool operator!=(const const_iterator_base& other) const = 0;
        
        /**
         * @brief Equality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are equal
         */
        virtual bool operator==(const typename fwd_container<T>::iterator_base& other) const = 0;
        
        /**
         * @brief Inequality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are not equal
         */
        virtual bool operator!=(const typename fwd_container<T>::iterator_base& other) const = 0;

    protected:
        /**
         * @brief Clone this iterator
         * @return Pointer to new cloned const_iterator_base
         */
        virtual const_iterator_base* clone() const = 0;
        
        friend class iterator;
        friend class const_iterator;
    };

    /**
     * @brief Non-const forward iterator for the container
     */
    class iterator {
    private:
        iterator_base* ptr; ///< Pointer to the base iterator implementation

    public:
        using difference_type   = ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = std::forward_iterator_tag;

        /**
         * @brief Default constructor
         */
        iterator() : ptr(nullptr) {}
        
        /**
         * @brief Constructor from base iterator pointer
         * @param p Pointer to iterator_base implementation
         */
        iterator(iterator_base* p) : ptr(p) {}
        
        /**
         * @brief Copy constructor
         * @param other Iterator to copy from
         */
        iterator(const iterator& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}
        
        /**
         * @brief Move constructor
         * @param other Iterator to move from
         */
        iterator(iterator&& other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }
        
        /**
         * @brief Constructor from const_iterator
         * @param other Const iterator to convert from
         */
        iterator(const const_iterator& other) : ptr(nullptr) {}
        
        /**
         * @brief Destructor
         */
        ~iterator() {
            delete ptr;
        }
        
        /**
         * @brief Copy assignment operator
         * @param other Iterator to assign from
         * @return Reference to this iterator
         */
        iterator& operator=(const iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->clone() : nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Move assignment operator
         * @param other Iterator to move from
         * @return Reference to this iterator
         */
        iterator& operator=(iterator&& other) noexcept {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Assignment from const_iterator
         * @param other Const iterator to assign from
         * @return Reference to this iterator
         */
        iterator& operator=(const const_iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Dereference operator
         * @return Reference to the current element
         */
        T& operator*() {
            return **ptr;
        }
        
        /**
         * @brief Member access operator
         * @return Pointer to the current element
         */
        T* operator->() {
            return &(**ptr);
        }
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        iterator& operator++() {
            ++(*ptr);
            return *this;
        }
        
        /**
         * @brief Postfix increment operator
         * @return Copy of iterator before increment
         */
        iterator operator++(int) {
            iterator temp = *this;
            ++(*ptr);
            return temp;
        }
        
        /**
         * @brief Equality comparison with iterator
         * @param other Iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        /**
         * @brief Inequality comparison with iterator
         * @param other Iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        /**
         * @brief Equality comparison with const_iterator
         * @param other Const iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const const_iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        /**
         * @brief Inequality comparison with const_iterator
         * @param other Const iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        friend class const_iterator;
    };

    /**
     * @brief Const forward iterator for the container
     */
    class const_iterator {
    private:
        const_iterator_base* ptr; ///< Pointer to the base const iterator implementation

    public:
        using difference_type   = ptrdiff_t;
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;
        using iterator_category = std::forward_iterator_tag;

        /**
         * @brief Default constructor
         */
        const_iterator() : ptr(nullptr) {}
        
        /**
         * @brief Constructor from base const iterator pointer
         * @param p Pointer to const_iterator_base implementation
         */
        const_iterator(const_iterator_base* p) : ptr(p) {}
        
        /**
         * @brief Copy constructor
         * @param other Const iterator to copy from
         */
        const_iterator(const const_iterator& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}
        
        /**
         * @brief Move constructor
         * @param other Const iterator to move from
         */
        const_iterator(const_iterator&& other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }
        
        /**
         * @brief Constructor from non-const iterator
         * @param other Iterator to convert from
         */
        const_iterator(const iterator& other) : ptr(nullptr) {
            if (other.ptr) {
                // Use create_const() method for conversion
                ptr = other.ptr->create_const();
            }
        }
        
        /**
         * @brief Destructor
         */
        ~const_iterator() {
            delete ptr;
        }
        
        /**
         * @brief Copy assignment operator
         * @param other Const iterator to assign from
         * @return Reference to this iterator
         */
        const_iterator& operator=(const const_iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->clone() : nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Move assignment operator
         * @param other Const iterator to move from
         * @return Reference to this iterator
         */
        const_iterator& operator=(const_iterator&& other) noexcept {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Assignment from non-const iterator
         * @param other Iterator to assign from
         * @return Reference to this iterator
         */
        const_iterator& operator=(const iterator& other) {
            if (this != &other) {
                delete ptr;
                ptr = other.ptr ? other.ptr->create_const() : nullptr;
            }
            return *this;
        }
        
        /**
         * @brief Dereference operator
         * @return Const reference to the current element
         */
        const T& operator*() const {
            return **ptr;
        }
        
        /**
         * @brief Member access operator
         * @return Const pointer to the current element
         */
        const T* operator->() const {
            return &(**ptr);
        }
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        const_iterator& operator++() {
            ++(*ptr);
            return *this;
        }
        
        /**
         * @brief Postfix increment operator
         * @return Copy of iterator before increment
         */
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*ptr);
            return temp;
        }
        
        /**
         * @brief Equality comparison with const_iterator
         * @param other Const iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const const_iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        /**
         * @brief Inequality comparison with const_iterator
         * @param other Const iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        /**
         * @brief Equality comparison with iterator
         * @param other Iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const iterator& other) const {
            if (!ptr && !other.ptr) return true;
            if (!ptr || !other.ptr) return false;
            return *ptr == *other.ptr;
        }
        
        /**
         * @brief Inequality comparison with iterator
         * @param other Iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        
        friend class iterator;
    };

    /**
     * @brief Add element to the container (copy semantics)
     * @param value The value to add
     */
    virtual void push(const T& value) = 0;
    
    /**
     * @brief Add element to the container (move semantics)
     * @param value The value to add
     */
    virtual void push(T&& value) = 0;
    
    /**
     * @brief Remove and return element from the container
     * @return The removed element
     */
    virtual T pop() = 0;
    
    /**
     * @brief Get reference to the front element
     * @return Reference to the front element
     */
    virtual T& get_front() = 0;
    
    /**
     * @brief Get const reference to the front element
     * @return Const reference to the front element
     */
    virtual const T& get_front() const = 0;
    
    /**
     * @brief Check if container is empty
     * @return True if container is empty
     */
    virtual bool is_empty() const = 0;
    
    /**
     * @brief Get the number of elements in container
     * @return Size of the container
     */
    virtual size_t size() const = 0;
    
    /**
     * @brief Get iterator to the beginning
     * @return Iterator to the first element
     */
    virtual iterator begin() = 0;
    
    /**
     * @brief Get iterator to the end
     * @return Iterator to the position after the last element
     */
    virtual iterator end() = 0;
    
    /**
     * @brief Get const iterator to the beginning
     * @return Const iterator to the first element
     */
    virtual const_iterator begin() const = 0;
    
    /**
     * @brief Get const iterator to the end
     * @return Const iterator to the position after the last element
     */
    virtual const_iterator end() const = 0;
    
    /**
     * @brief Get const iterator to the beginning
     * @return Const iterator to the first element
     */
    virtual const_iterator cbegin() const = 0;
    
    /**
     * @brief Get const iterator to the end
     * @return Const iterator to the position after the last element
     */
    virtual const_iterator cend() const = 0;

    /**
     * @brief Assignment operator
     * @param other Container to assign from
     * @return Reference to this container
     */
    virtual fwd_container<T>& operator=(const fwd_container<T>& other) = 0;

    /**
     * @brief Virtual destructor
     */
    virtual ~fwd_container() = default;

    /**
     * @brief Output stream operator
     * @param os Output stream
     * @param container Container to output
     * @return Reference to the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const fwd_container<T>& container) {
        try {
            container.print(os);   
            return os;
        }
        catch(std::runtime_error& e) {
            throw;
        }
    }

    /**
     * @brief Input stream operator
     * @param is Input stream
     * @param container Container to read into
     * @return Reference to the input stream
     */
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
    /**
     * @brief Print container contents to output stream
     * @param os Output stream
     * @return Reference to the output stream
     */
    virtual std::ostream& print(std::ostream& os) const = 0;
    
    /**
     * @brief Read container contents from input stream
     * @param is Input stream
     * @return Reference to the input stream
     */
    virtual std::istream& read(std::istream& is) = 0;
};