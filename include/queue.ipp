#include "queue.h"

// Queue constructors and operators
template<typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}

template<typename T>
Queue<T>::Queue(const Queue<T>& other) : frontNode(nullptr), rearNode(nullptr), queueSize(0) {
    if (!other.is_empty()) {
        Node<T>* current = other.frontNode;   

        try {         
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }         
        } 
        catch (const std::bad_alloc& e) {
            clear();
            throw std::runtime_error("Memory allocation failed during copy construction: " + std::string(e.what()));
        }
    }
}

template<typename T>
Queue<T>::Queue(Queue<T>&& other) : frontNode(other.frontNode), rearNode(other.rearNode), queueSize(other.queueSize) {
    other.frontNode = nullptr;
    other.rearNode = nullptr;
    other.queueSize = 0;
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    if (this != &other) {
        clear();
        queueSize = 0;   
        Node<T>* current = other.frontNode;

        try {
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }    
        }
        catch(const std::bad_alloc& e) {
            throw std::runtime_error("Memory allocation failed during copy assignment: " + std::string(e.what()));
        }
    }
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue<T>&& other) {
    if (this != &other) {
        clear();
        frontNode = other.frontNode;
        rearNode = other.rearNode;
        queueSize = other.queueSize;
        
        other.frontNode = nullptr;
        other.rearNode = nullptr;
        other.queueSize = 0;
    }
    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(const fwd_container<T>& other){
    const Queue<T>* derived = dynamic_cast<const Queue<T>*>(&other);
    if (!derived) {
        throw std::bad_cast();
    }
    return *this = *derived; // Use same-type assignment operator
}


template<typename T>
Queue<T>::~Queue() {
    clear();
}

// fwd_container interface implementation
template<typename T>
void Queue<T>::push(const T& value) {
    try {
        Node<T>* newNode = new Node<T>(value); // next = nullptr
        
        if (is_empty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode; 
            rearNode = newNode;       
        }
        ++queueSize;
    }
    catch(std::bad_alloc& e) {
        throw std::runtime_error("Failed to allocate memory for new queue element: " + std::string(e.what()));
    } 
}

template<typename T>
void Queue<T>::push(T&& value) { 
    try {
        Node<T>* newNode = new Node<T>(std::move(value)); // next = nullptr
        
        if (is_empty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode; 
            rearNode = newNode;       
        }
        ++queueSize;
    }
    catch(std::bad_alloc& e) {
        throw std::runtime_error("Failed to allocate memory for new queue element: " + std::string(e.what()));
    } 
}

template<typename T>
T Queue<T>::pop() {
    if (is_empty()) throw std::runtime_error("Cannot pop: Queue is empty");

    Node<T>* temp = frontNode;
    frontNode = frontNode->next;
    if(frontNode == nullptr) rearNode = nullptr;
    T value = std::move(temp->data); 
    --queueSize;  
    delete temp;
    return value;
}

template<typename T>
T& Queue<T>::get_front() {
    if (is_empty()) throw std::runtime_error("Cannot get front data: Queue is empty");
    return frontNode->data;
}

template<typename T>
const T& Queue<T>::get_front() const {
    if (is_empty()) throw std::runtime_error("Cannot get front data: Queue is empty");
    return frontNode->data;
}

template<typename T>
bool Queue<T>::is_empty() const {
    return frontNode == nullptr;
}

template<typename T>
size_t Queue<T>::size() const {
    return queueSize;
}

template<typename T>
typename Queue<T>::iterator Queue<T>::begin() {
    return iterator(new queue_iterator(frontNode));
}

template<typename T>
typename Queue<T>::iterator Queue<T>::end() {
    return iterator(new queue_iterator(nullptr));
}

template<typename T>
typename Queue<T>::const_iterator Queue<T>::begin() const {
    return const_iterator(new queue_const_iterator(frontNode));
}

template<typename T>
typename Queue<T>::const_iterator Queue<T>::end() const {
    return const_iterator(new queue_const_iterator(nullptr));
}

template<typename T>
typename Queue<T>::const_iterator Queue<T>::cbegin() const {
    return const_iterator(new queue_const_iterator(frontNode));
}

template<typename T>
typename Queue<T>::const_iterator Queue<T>::cend() const {
    return const_iterator(new queue_const_iterator(nullptr));
}

// Additional methods for backward compatibility
template<typename T>
size_t Queue<T>::getSize() const {
    return queueSize;
}

template<typename T>
Node<T>* Queue<T>::getFrontNode() {
    if (is_empty()) throw std::runtime_error("Cannot get front Node: Queue is empty");
    return frontNode;
}

template<typename T>
auto Queue<T>::getFrontNode() const -> const Node<T>* {
    if (is_empty()) throw std::runtime_error("Cannot get front Node: Queue is empty");
    return frontNode;
}

template<typename T>
Node<T>* Queue<T>::getRearNode() {
    if (is_empty()) throw std::runtime_error("Cannot get rear Node: Queue is empty");
    return rearNode;
}

template<typename T>
auto Queue<T>::getRearNode() const -> const Node<T>* {
    if (is_empty()) throw std::runtime_error("Cannot get rear Node: Queue is empty");
    return rearNode;
}

template<typename T>
T& Queue<T>::front() {
    return get_front();
}

template<typename T>
const T& Queue<T>::front() const {
    return get_front();
}

template<typename T>
void Queue<T>::clear() {
    while (!is_empty()) {
        pop();
    }
}

template<typename T>
bool Queue<T>::empty() const {
    return is_empty();
}

// Protected methods for stream operations
template<typename T>
std::ostream& Queue<T>::print(std::ostream& os) const {
    try {
        if (!os.good()) throw std::runtime_error("Output stream is in bad state");
        
        Node<T>* current = frontNode;  
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
        throw std::runtime_error(std::string("Queue serialization failed: ") + e.what());
    }
}

template<typename T>
std::istream& Queue<T>::read(std::istream& is) {
    try {
        if (!is.good()) {
            throw std::runtime_error("Input stream is in bad state");
        }
        
        Queue<T> backup = *this;
        
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
        throw std::runtime_error(std::string("Queue input failed: ") + e.what());
    }
}