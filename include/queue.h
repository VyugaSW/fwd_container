#pragma once
#include <iostream>
#include <exception>
#include <sstream>

#include "Node.h"
#include "fwd_container.h"

/**
 * @brief A queue implementation using linked nodes
 * @tparam T The type of elements stored in the queue
 * 
 * This class implements a FIFO (First-In-First-Out) data structure
 * using a singly linked list of nodes.
 */
template<typename T>
class Queue : public fwd_container<T> {
public:
    using iterator = typename fwd_container<T>::iterator;
    using const_iterator = typename fwd_container<T>::const_iterator;

    /**
     * @brief Default constructor - creates an empty queue
     */
    Queue();

    /**
     * @brief Copy constructor - creates a deep copy of another queue
     * @param other Queue to copy from
     * @throws std::runtime_error if memory allocation fails during copying
     */
    Queue(const Queue<T>& other);

    /**
     * @brief Move constructor - transfers ownership from another queue
     * @param other Queue to move from (will be left in valid but empty state)
     */
    Queue(Queue<T>&& other);

    /**
     * @brief Copy assignment operator
     * @param other Queue to copy from
     * @return Reference to this queue
     * @throws std::runtime_error if memory allocation fails during copying
     */
    Queue<T>& operator=(const Queue<T>& other);

    /**
     * @brief Move assignment operator
     * @param other Queue to move from
     * @return Reference to this queue
     */
    Queue<T>& operator=(Queue<T>&& other);

    /**
     * @brief Virtual destructor
     */
    ~Queue();

    // fwd_container interface implementation
    /**
     * @brief Add element to the back of the queue (copy semantics)
     * @param value The value to add
     */
    void push(const T& value) override;
    
    /**
     * @brief Add element to the back of the queue (move semantics)
     * @param value The value to add
     */
    void push(T&& value) override;
    
    /**
     * @brief Remove and return element from the front of the queue
     * @return The removed element from front
     * @throws std::runtime_error if queue is empty
     */
    T pop() override;
    
    /**
     * @brief Get reference to the front element
     * @return Reference to the front element
     * @throws std::runtime_error if queue is empty
     */
    T& get_front() override;
    
    /**
     * @brief Get const reference to the front element
     * @return Const reference to the front element
     * @throws std::runtime_error if queue is empty
     */
    const T& get_front() const override;
    
    /**
     * @brief Check if queue is empty
     * @return True if queue is empty, false otherwise
     */
    bool is_empty() const override;
    
    /**
     * @brief Get the number of elements in queue
     * @return Size of the queue
     */
    size_t size() const override;

    /**
     * @brief Assignment from any fwd_container
     * @param other Container to copy from
     * @return Reference to this queue
     * @throws std::runtime_error if memory allocation fails
     */
    Queue<T>& operator=(const fwd_container<T>& other) override;
    
    /**
     * @brief Get iterator to the beginning of the queue
     * @return Iterator to the first element
     */
    iterator begin() override;
    
    /**
     * @brief Get iterator to the end of the queue
     * @return Iterator to the position after the last element
     */
    iterator end() override;
    
    /**
     * @brief Get const iterator to the beginning of the queue
     * @return Const iterator to the first element
     */
    const_iterator begin() const override;
    
    /**
     * @brief Get const iterator to the end of the queue
     * @return Const iterator to the position after the last element
     */
    const_iterator end() const override;
    
    /**
     * @brief Get const iterator to the beginning of the queue
     * @return Const iterator to the first element
     */
    const_iterator cbegin() const override;
    
    /**
     * @brief Get const iterator to the end of the queue
     * @return Const iterator to the position after the last element
     */
    const_iterator cend() const override;

    /**
     * @brief Returns the number of elements in the queue
     * @return Current size of the queue
     */
    size_t getSize() const;
    
    /**
     * @brief Returns a pointer to the front node (non-const version)
     * @return Pointer to the front node
     * @throws std::runtime_error if queue is empty
     */
    Node<T>* getFrontNode();

    /**
     * @brief Returns a pointer to the front node (const version)
     * @return Const pointer to the front node
     * @throws std::runtime_error if queue is empty
     */
    auto getFrontNode() const -> const Node<T>*;

    /**
     * @brief Returns a pointer to the rear node (non-const version)
     * @return Pointer to the rear node
     * @throws std::runtime_error if queue is empty
     */
    Node<T>* getRearNode();

    /**
     * @brief Returns a pointer to the rear node (const version)
     * @return Const pointer to the rear node
     * @throws std::runtime_error if queue is empty
     */
    auto getRearNode() const -> const Node<T>*;

    /**
     * @brief Returns a reference to the front element (non-const version)
     * @return Reference to the front element
     * @throws std::runtime_error if queue is empty
     */
    T& front();

    /**
     * @brief Returns a reference to front element (const version)
     * @return Const reference to front element
     * @throws std::runtime_error if queue is empty
     */
    const T& front() const;

    /**
     * @brief Removes all elements from the queue
     */
    void clear();

    /**
     * @brief Checks if the queue is empty
     * @return true if queue is empty, false otherwise
     */
    bool empty() const;

protected:
    /**
     * @brief Print queue contents to output stream
     * @param os Output stream
     * @return Reference to the output stream
     */
    virtual std::ostream& print(std::ostream& os) const override;
    
    /**
     * @brief Read queue contents from input stream
     * @param is Input stream
     * @return Reference to the input stream
     */
    virtual std::istream& read(std::istream& is) override;

private:
    /**
     * @brief Iterator implementation for Queue (non-const version)
     */
    class queue_iterator : public fwd_container<T>::iterator_base {
    private:
        Node<T>* current; ///< Current node pointer

    public:
        /**
         * @brief Constructor
         * @param node Starting node for iteration
         */
        queue_iterator(Node<T>* node) : current(node) {}
        
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
        queue_iterator& operator++() override {
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
            const queue_iterator* derived = dynamic_cast<const queue_iterator*>(&other);
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
            const queue_const_iterator* derived = dynamic_cast<const queue_const_iterator*>(&other);
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
            return new queue_const_iterator(current);
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
        queue_iterator* clone() const override {
            return new queue_iterator(current);
        }
        
        friend class queue_const_iterator;
    };

    /**
     * @brief Const iterator implementation for Queue
     */
    class queue_const_iterator : public fwd_container<T>::const_iterator_base {
    private:
        const Node<T>* current; ///< Current node pointer

    public:
        /**
         * @brief Constructor
         * @param node Starting node for iteration
         */
        queue_const_iterator(const Node<T>* node) : current(node) {}
        
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
        queue_const_iterator& operator++() override {
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
            const queue_const_iterator* derived = dynamic_cast<const queue_const_iterator*>(&other);
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
            const queue_iterator* derived = dynamic_cast<const queue_iterator*>(&other);
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
        queue_const_iterator* clone() const override {
            return new queue_const_iterator(current);
        }

        friend class queue_iterator;
    };

    Node<T>* frontNode; ///< Pointer to the front node (for pop operations)
    Node<T>* rearNode;  ///< Pointer to the rear node (for push operations)
    size_t queueSize;   ///< Number of elements in the queue
};

#include "queue.ipp"