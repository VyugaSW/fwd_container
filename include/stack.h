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
    void push(const T& value) override;
    void push(T&& value) override;
    T pop() override;
    
    T& get_front() override;
    const T& get_front() const override;
    
    bool is_empty() const override;
    size_t size() const override;

    Stack<T>& operator=(const fwd_container<T>& other) override;
    
    iterator begin() override;
    iterator end() override;
    const_iterator begin() const override;
    const_iterator end() const override;
    const_iterator cbegin() const override;
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
    virtual std::ostream& print(std::ostream& os) const override;
    virtual std::istream& read(std::istream& is) override;

private:
    class stack_iterator : public fwd_container<T>::iterator_base {
    private:
        Node<T>* current;

    public:
        stack_iterator(Node<T>* node) : current(node) {}
        
        T& operator*() override {
            return current->data;
        }
        
        T* operator->() override {
            return &(current->data);
        }
        
        stack_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            const stack_iterator* derived = dynamic_cast<const stack_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }
        
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            const stack_const_iterator* derived = dynamic_cast<const stack_const_iterator*>(&other);
            return derived && current == derived->get_current();
        }
        
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }

        typename fwd_container<T>::const_iterator_base* create_const() const override {
            return new stack_const_iterator(current);
        }

        Node<T>* get_current() const { return current; }

    protected:
        stack_iterator* clone() const override {
            return new stack_iterator(current);
        }
        
        friend class stack_const_iterator;
    };

    class stack_const_iterator : public fwd_container<T>::const_iterator_base {
    private:
        const Node<T>* current;

    public:
        stack_const_iterator(const Node<T>* node) : current(node) {}
        
        const T& operator*() const override {
            return current->data;
        }
        
        const T* operator->() const override {
            return &(current->data);
        }
        
        stack_const_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            const stack_const_iterator* derived = dynamic_cast<const stack_const_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }
        
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            const stack_iterator* derived = dynamic_cast<const stack_iterator*>(&other);
            return derived && current == derived->get_current(); // Используем геттер
        }
        
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }

        const Node<T>* get_current() const { return current; }

    protected:
        stack_const_iterator* clone() const override {
            return new stack_const_iterator(current);
        }

        friend class stack_iterator;
    };

    Node<T>* topNode;    ///< Pointer to the top node of the stack
    size_t stackSize;    ///< Number of elements in the stack
};

#include "stack.ipp"