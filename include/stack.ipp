#include "stack.h"

// Stack constructors and operators
template<typename T>
Stack<T>::Stack() : topNode(nullptr), stackSize(0) {}

template<typename T>
Stack<T>::Stack(const Stack<T>& other) : topNode(nullptr), stackSize(other.stackSize) {
    if (!other.is_empty()) {
        Node<T>* current = other.topNode;
        Node<T>* prevNewNode = nullptr;
        Node<T>* newTopNode = nullptr;
        
        try {
            newTopNode = new Node<T>(current->data);
            prevNewNode = newTopNode;
            current = current->next;
            
            while (current != nullptr) {
                Node<T>* newNode = new Node<T>(current->data);
                prevNewNode->next = newNode; 
                prevNewNode = newNode;        
                current = current->next;
            }
            
            topNode = newTopNode; 
            
        } 
        catch (const std::bad_alloc& e) {
            while (newTopNode != nullptr) {
                Node<T>* temp = newTopNode;
                newTopNode = newTopNode->next;
                delete temp;
            }
            throw std::runtime_error("Memory allocation failed during copy construction: " + std::string(e.what()));
        }
    }
}

template<typename T>
Stack<T>::Stack(Stack<T>&& other) : topNode(other.topNode), stackSize(other.stackSize) {
    other.topNode = nullptr;
    other.stackSize = 0;
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
    if (this != &other) {
        clear();
        stackSize = 0;
        
        if (!other.is_empty()) {
            Node<T>* current = other.topNode;
            Node<T>* prevNewNode = nullptr;
            Node<T>* newTopNode = nullptr;
            
            try {
                newTopNode = new Node<T>(current->data);
                prevNewNode = newTopNode;
                current = current->next;
                
                while (current != nullptr) {
                    Node<T>* newNode = new Node<T>(current->data);
                    prevNewNode->next = newNode; 
                    prevNewNode = newNode;        
                    current = current->next;
                }
                
                topNode = newTopNode; 
                stackSize = other.stackSize;
            }
            catch(const std::bad_alloc& e) {
                // Clean up and rethrow on allocation failure
                while (newTopNode != nullptr) {
                    Node<T>* temp = newTopNode;
                    newTopNode = newTopNode->next;
                    delete temp;
                }
                throw std::runtime_error("Memory allocation failed during copy assignment: " + std::string(e.what()));
            }
        }
    }
    return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack<T>&& other) {
    if (this != &other) {
        clear();
        topNode = other.topNode;
        stackSize = other.stackSize;
        
        other.topNode = nullptr;
        other.stackSize = 0;
    }
    return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator=(const fwd_container<T>& other){
    const Stack<T>* derived = dynamic_cast<const Stack<T>*>(&other);
    if (!derived) {
        throw std::bad_cast();
    }
    return *this = *derived; 
}


template<typename T>
Stack<T>::~Stack() {
    clear();
}

// fwd_container interface implementation
template<typename T>
void Stack<T>::push(const T& value) {
    try {
        Node<T>* newNode = new Node<T>(value, topNode);
        topNode = newNode;
        ++stackSize; 
    }
    catch(std::bad_alloc& e) {
        throw std::runtime_error("Failed to allocate memory for new stack element: " + std::string(e.what()));
    } 
}

template<typename T>
void Stack<T>::push(T&& value) { 
    try {
        Node<T>* newNode = new Node<T>(std::move(value), topNode);
        topNode = newNode;
        ++stackSize; 
    }
    catch(std::bad_alloc& e) {
        throw std::runtime_error("Failed to allocate memory for new stack element: " + std::string(e.what()));
    } 
}

template<typename T>
T Stack<T>::pop() {
    if (is_empty()) throw std::runtime_error("Cannot pop: Stack is empty");

    Node<T>* temp = topNode;
    topNode = topNode->next;
    T value = std::move(temp->data); 
    --stackSize;  
    delete temp;
    return value;
}

template<typename T>
T& Stack<T>::get_front() {
    if (is_empty()) throw std::runtime_error("Cannot get top data: Stack is empty");
    return topNode->data;
}

template<typename T>
const T& Stack<T>::get_front() const {
    if (is_empty()) throw std::runtime_error("Cannot get top data: Stack is empty");
    return topNode->data;
}

template<typename T>
bool Stack<T>::is_empty() const {
    return topNode == nullptr;
}

template<typename T>
size_t Stack<T>::size() const {
    return stackSize;
}

template<typename T>
typename Stack<T>::iterator Stack<T>::begin() {
    return iterator(new stack_iterator(topNode));
}

template<typename T>
typename Stack<T>::iterator Stack<T>::end() {
    return iterator(new stack_iterator(nullptr));
}

template<typename T>
typename Stack<T>::const_iterator Stack<T>::begin() const {
    return const_iterator(new stack_const_iterator(topNode));
}

template<typename T>
typename Stack<T>::const_iterator Stack<T>::end() const {
    return const_iterator(new stack_const_iterator(nullptr));
}

template<typename T>
typename Stack<T>::const_iterator Stack<T>::cbegin() const {
    return const_iterator(new stack_const_iterator(topNode));
}

template<typename T>
typename Stack<T>::const_iterator Stack<T>::cend() const {
    return const_iterator(new stack_const_iterator(nullptr));
}

// Additional methods for backward compatibility
template<typename T>
size_t Stack<T>::getSize() const {
    return stackSize;
}

template<typename T>
bool Stack<T>::empty() const {
    return is_empty();
}

template<typename T>
T& Stack<T>::top() {
    return get_front();
}

template<typename T>
const T& Stack<T>::top() const {
    return get_front();
}

template<typename T>
void Stack<T>::clear() {
    while (!is_empty()) {
        pop();
    }
}

// Protected methods for stream operations
template<typename T>
std::ostream& Stack<T>::print(std::ostream& os) const {
    try {
        if (!os.good()) throw std::runtime_error("Output stream is in bad state");
        
        Node<T>* current = topNode;  
        bool first = true;
        
        while (current != nullptr) {
            if (!first) {
                os << " ";
            }
            
            if (!os.good()) throw std::runtime_error("Output stream failed during serialization");
            
            os << current->data;
            current = current->next;
            first = false;
        }
        
        if (!os.good()) throw std::runtime_error("Output stream failed after serialization");
        
        return os;
        
    } catch (const std::exception& e) {
        os.setstate(std::ios::failbit);
        throw std::runtime_error(std::string("Stack serialization failed: ") + e.what());
    }
}

template<typename T>
std::istream& Stack<T>::read(std::istream& is) {
    try {
        if (!is.good()) {
            throw std::runtime_error("Input stream is in bad state");
        }
        
        Stack<T> backup = *this;
        
        try {
            T value;
            while (is >> value) {
                try {
                    this->push(value);
                } 
                catch (const std::bad_alloc& e) {
                    throw std::runtime_error("Memory allocation failed during input: " + std::string(e.what()));
                } 
                catch (const std::exception& e) {
                    throw std::runtime_error("Push operation failed during input: " + std::string(e.what()));
                }
                
                if (!is.good() && !is.eof()) {
                    throw std::runtime_error("Input stream failed during data reading");
                }
            }

            if (is.eof()) {
                is.clear();
            }

            if (is.fail() && !is.eof()) {
                throw std::runtime_error("Failed to parse input data");
            }
            
            return is;
            
        } 
        catch (...) {
            *this = std::move(backup);
            throw;
        }
        
    } 
    catch (const std::exception& e) 
    {
        is.setstate(std::ios::failbit);
        throw std::runtime_error(std::string("Stack input failed: ") + e.what());
    }
}