#pragma once
#include <iostream>
#include <exception>
#include <sstream>

#include "Node.h"
#include "fwd_container.h"

template<typename T>
class Queue : public fwd_container<T> {
public:
    using iterator = typename fwd_container<T>::iterator;
    using const_iterator = typename fwd_container<T>::const_iterator;

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
    void push(const T& value) override;
    void push(T&& value) override;
    T pop() override;
    
    T& get_front() override;
    const T& get_front() const override;
    
    bool is_empty() const override;
    size_t size() const override;

    Queue<T>& operator=(const fwd_container<T>& other) override;
    
    iterator begin() override;
    iterator end() override;
    const_iterator begin() const override;
    const_iterator end() const override;
    const_iterator cbegin() const override;
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
    virtual std::ostream& print(std::ostream& os) const override;
    virtual std::istream& read(std::istream& is) override;

private:

    class queue_iterator : public fwd_container<T>::iterator_base {
    private:
        Node<T>* current;

    public:
        queue_iterator(Node<T>* node) : current(node) {}
        
        T& operator*() override {
            return current->data;
        }
        
        T* operator->() override {
            return &(current->data);
        }
        
        queue_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            const queue_iterator* derived = dynamic_cast<const queue_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }
        
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            const queue_const_iterator* derived = dynamic_cast<const queue_const_iterator*>(&other);
            return derived && current == derived->get_current(); 
        }
        
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }

        typename fwd_container<T>::const_iterator_base* create_const() const override {
            return new queue_const_iterator(current);
        }

        Node<T>* get_current() const { return current; }

    protected:
        queue_iterator* clone() const override {
            return new queue_iterator(current);
        }
        
        friend class queue_const_iterator;
    };

    class queue_const_iterator : public fwd_container<T>::const_iterator_base {
    private:
        const Node<T>* current;

    public:
        queue_const_iterator(const Node<T>* node) : current(node) {}
        
        const T& operator*() const override {
            return current->data;
        }
        
        const T* operator->() const override {
            return &(current->data);
        }
        
        queue_const_iterator& operator++() override {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        
        bool operator==(const typename fwd_container<T>::const_iterator_base& other) const override {
            const queue_const_iterator* derived = dynamic_cast<const queue_const_iterator*>(&other);
            return derived && current == derived->current;
        }
        
        bool operator!=(const typename fwd_container<T>::const_iterator_base& other) const override {
            return !(*this == other);
        }
        
        bool operator==(const typename fwd_container<T>::iterator_base& other) const override {
            const queue_iterator* derived = dynamic_cast<const queue_iterator*>(&other);
            return derived && current == derived->get_current(); 
        }
        
        bool operator!=(const typename fwd_container<T>::iterator_base& other) const override {
            return !(*this == other);
        }

        const Node<T>* get_current() const { return current; }

    protected:
        queue_const_iterator* clone() const override {
            return new queue_const_iterator(current);
        }

        friend class queue_iterator;
    };

    Node<T>* frontNode; // begin for pop
    Node<T>* rearNode; // end for push
    size_t queueSize;
};

#include "queue.ipp"