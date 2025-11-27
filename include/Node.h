#pragma once

/**
 * @brief Node structure for Stack implementation using singly linked list
 * @tparam T Type of data stored in the node
 */
template<typename T>
struct Node {
    T data;           ///< Data stored in the node
    Node<T>* next;    ///< Pointer to the next node in the stack
    
    /**
     * @brief Constructs a new Node with copy semantics
     * @param value Data value to store
     * @param next_node Pointer to the next node (default: nullptr)
     */
    Node(const T& value, Node<T>* next_node = nullptr);
    
    /**
     * @brief Constructs a new Node with move semantics
     * @param value Data value to move
     * @param next_node Pointer to the next node (default: nullptr)
     */
    Node(T&& value, Node<T>* next_node = nullptr);
};

#include "Node.ipp"