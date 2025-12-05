#pragma once
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <new>

#include "Node.h"
#include "fwd_container.h"

/**
 * @brief Stack implementation using singly linked list
 * @tparam T Type of elements stored in the stack
 * 
 * This class implements a LIFO (Last-In-First-Out) data structure
 * using a singly linked list of nodes.
 */
template<typename T>
class Stack : public fwd_container<T> {
public:
    using iterator = typename fwd_container<T>::iterator;
    using const_iterator = typename fwd_container<T>::const_iterator;

    /**
     * @brief Default constructor - creates an empty stack
     */
    Stack();

    /**
     * @brief Copy constructor - creates a deep copy of another stack
     * @param other Stack to copy from
     * @throws std::runtime_error if memory allocation fails during copying
     */
    Stack(const Stack<T>& other);

    /**
     * @brief Move constructor - transfers ownership from another stack
     * @param other Stack to move from (will be left in valid but empty state)
     */
    Stack(Stack<T>&& other);

    /**
     * @brief Copy assignment operator
     * @param other Stack to copy from
     * @return Reference to this stack
     * @throws std::runtime_error if memory allocation fails during copying
     */
    Stack<T>& operator=(const Stack<T>& other);

    /**
     * @brief Move assignment operator
     * @param other Stack to move from
     * @return Reference to this stack
     */
    Stack<T>& operator=(Stack<T>&& other);

    /**
     * @brief Virtual destructor
     */
    ~Stack();

    // fwd_container interface implementation
    /**
     * @brief Add element to the top of the stack (copy semantics)
     * @param value The value to add
     */
    void push(const T& value) override;
    
    /**
     * @brief Add element to the top of the stack (move semantics)
     * @param value The value to add
     */
    void push(T&& value) override;
    
    /**
     * @brief Remove and return element from the top of the stack
     * @return The removed element from top
     * @throws std::runtime_error if stack is empty
     */
    T pop() override;
    
    /**
     * @brief Get reference to the top element
     * @return Reference to the top element
     * @throws std::runtime_error if stack is empty
     */
    T& get_front() override;
    
    /**
     * @brief Get const reference to the top element
     * @return Const reference to the top element
     * @throws std::runtime_error if stack is empty
     */
    const T& get_front() const override;
    
    /**
     * @brief Check if stack is empty
     * @return True if stack is empty, false otherwise
     */
    bool is_empty() const override;
    
    /**
     * @brief Get the number of elements in stack
     * @return Size of the stack
     */
    size_t size() const override;

    /**
     * @brief Assignment from any fwd_container
     * @param other Container to copy from
     * @return Reference to this stack
     * @throws std::runtime_error if memory allocation fails
     */
    Stack<T>& operator=(const fwd_container<T>& other) override;
    
    /**
     * @brief Get iterator to the beginning of the stack
     * @return Iterator to the first element (top of stack)
     */
    iterator begin() override;
    
    /**
     * @brief Get iterator to the end of the stack
     * @return Iterator to the position after the last element
     */
    iterator end() override;
    
    /**
     * @brief Get const iterator to the beginning of the stack
     * @return Const iterator to the first element (top of stack)
     */
    const_iterator begin() const override;
    
    /**
     * @brief Get const iterator to the end of the stack
     * @return Const iterator to the position after the last element
     */
    const_iterator end() const override;
    
    /**
     * @brief Get const iterator to the beginning of the stack
     * @return Const iterator to the first element (top of stack)
     */
    const_iterator cbegin() const override;
    
    /**
     * @brief Get const iterator to the end of the stack
     * @return Const iterator to the position after the last element
     */
    const_iterator cend() const override;

    /**
     * @brief Returns the number of elements in the stack
     * @return Current size of the stack
     */
    size_t getSize() const;
    
    /**
     * @brief Checks if the stack is empty
     * @return true if stack is empty, false otherwise
     */
    bool empty() const;

    /**
     * @brief Returns a reference to the top element (non-const version)
     * @return Reference to the top element
     * @throws std::runtime_error if stack is empty
     */
    T& top();

    /**
     * @brief Returns a reference to the top element (const version)
     * @return Const reference to the top element
     * @throws std::runtime_error if stack is empty
     */
    const T& top() const;

    /**
     * @brief Removes all elements from the stack
     */
    void clear();

protected:
    /**
     * @brief Print stack contents to output stream
     * @param os Output stream
     * @return Reference to the output stream
     */
    virtual std::ostream& print(std::ostream& os) const override;
    
    /**
     * @brief Read stack contents from input stream
     * @param is Input stream
     * @return Reference to the input stream
     */
    virtual std::istream& read(std::istream& is) override;

private:
    /**
     * @brief Iterator implementation for Stack (non-const version)
     */
    class stack_iterator : public fwd_container<T>::iterator_base {
    private:
        Node<T>* current; ///< Current node pointer
        static constexpr int iterator_kind = 2;

    public:
        int get_iterator_kind() const noexcept override {
            return iterator_kind;
        }

        /**
         * @brief Constructor
         * @param node Starting node for iteration
         */
        stack_iterator(Node<T>* node) : current(node) {}
        
        /**
         * @brief Dereference operator
         * @return Reference to the current element
         */
        T& operator*() override {
            return current->data;
        }
        
        /**
         * @brief Member access operator
         * @return Pointer to the current element
         */
        T* operator->() override {
            return &(current->data);
        }
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        stack_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        /**
         * @brief Equality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            if (this->get_iterator_kind() != other.get_iterator_kind()) return false;
            const stack_iterator* derived = static_cast<const stack_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        /**
         * @brief Inequality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }
        
        /**
         * @brief Equality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            if (this->get_iterator_kind() != other.get_iterator_kind()) return false;
            const stack_const_iterator* derived = static_cast<const stack_const_iterator*>(&other);
            return derived && current == derived->get_current();
        }
        
        /**
         * @brief Inequality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }

        /**
         * @brief Create a const version of this iterator
         * @return Pointer to new const_iterator_base
         */
        typename fwd_container<T>::const_iterator_base* create_const() const override {
            return new stack_const_iterator(current);
        }

        /**
         * @brief Get the current node pointer
         * @return Pointer to current node
         */
        Node<T>* get_current() const { return current; }

    protected:
        /**
         * @brief Clone this iterator
         * @return Pointer to new cloned iterator
         */
        stack_iterator* clone() const override {
            return new stack_iterator(current);
        }

        friend class stack_const_iterator;
    };

    /**
     * @brief Const iterator implementation for Stack
     */
    class stack_const_iterator : public fwd_container<T>::const_iterator_base {
    private:
        const Node<T>* current; ///< Current node pointer
        static constexpr int iterator_kind = 2;

    public:
        int get_iterator_kind() const noexcept override {
            return iterator_kind;
        }
        
        /**
         * @brief Constructor
         * @param node Starting node for iteration
         */
        stack_const_iterator(const Node<T>* node) : current(node) {}
        
        /**
         * @brief Dereference operator
         * @return Const reference to the current element
         */
        const T& operator*() const override {
            return current->data;
        }
        
        /**
         * @brief Member access operator
         * @return Const pointer to the current element
         */
        const T* operator->() const override {
            return &(current->data);
        }
        
        /**
         * @brief Prefix increment operator
         * @return Reference to this iterator
         */
        stack_const_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        /**
         * @brief Equality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            if (this->get_iterator_kind() != other.get_iterator_kind()) return false;
            const stack_const_iterator* derived = static_cast<const stack_const_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        /**
         * @brief Inequality comparison with const_iterator_base
         * @param other The const iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }
        
        /**
         * @brief Equality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are equal
         */
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            if (this->get_iterator_kind() != other.get_iterator_kind()) return false;
            const stack_iterator* derived = static_cast<const stack_iterator*>(&other);
            return derived && current == derived->get_current();
        }
        
        /**
         * @brief Inequality comparison with iterator_base
         * @param other The iterator to compare with
         * @return True if iterators are not equal
         */
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }

        /**
         * @brief Get the current node pointer
         * @return Const pointer to current node
         */
        const Node<T>* get_current() const { return current; }

    protected:
        /**
         * @brief Clone this iterator
         * @return Pointer to new cloned const iterator
         */
        stack_const_iterator* clone() const override {
            return new stack_const_iterator(current);
        }

        friend class stack_iterator;
    };

    Node<T>* topNode;    ///< Pointer to the top node of the stack
    size_t stackSize;    ///< Number of elements in the stack
};

#include "stack.ipp"