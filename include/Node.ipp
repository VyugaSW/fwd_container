#include "Node.h"

// Node constructors
template<typename T>
Node<T>::Node(const T& value, Node<T>* next_node) 
    : data(value), next(next_node) {}

template<typename T>
Node<T>::Node(T&& value, Node<T>* next_node) 
    : data(std::move(value)), next(next_node) {}
