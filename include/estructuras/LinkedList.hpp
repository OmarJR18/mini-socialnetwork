#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>

template <typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        Node(const T& _value)
            : value(_value), next(nullptr)
        {
        }
    };

    Node* m_head;
    Node* m_tail;
    std::size_t m_size;

    void clear();

public:
    LinkedList();
    ~LinkedList();

    LinkedList(const LinkedList& _other);
    LinkedList& operator=(const LinkedList& _other);

    void add_first(const T& _item);
    void add_last(const T& _item);
    bool remove_first();
    bool remove_item(const T& _item);
    int search(const T& _item) const;

    bool empty() const;
    std::size_t size() const;

    T& at(std::size_t _index);
    const T& at(std::size_t _index) const;
};

template <typename T>
LinkedList<T>::LinkedList()
    : m_head(nullptr), m_tail(nullptr), m_size(0)
{
}

template <typename T>
LinkedList<T>::~LinkedList()
{
    clear();
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& _other)
    : m_head(nullptr), m_tail(nullptr), m_size(0)
{
    Node* current = _other.m_head;
    while (current != nullptr) {
        add_last(current->value);
        current = current->next;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& _other)
{
    if (this != &_other) {
        clear();
        Node* current = _other.m_head;
        while (current != nullptr) {
            add_last(current->value);
            current = current->next;
        }
    }
    return *this;
}

template <typename T>
void LinkedList<T>::clear()
{
    Node* current = m_head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}

template <typename T>
void LinkedList<T>::add_first(const T& _item)
{
    Node* new_node = new Node(_item);
    new_node->next = m_head;
    m_head = new_node;

    if (m_tail == nullptr)
        m_tail = new_node;

    ++m_size;
}

template <typename T>
void LinkedList<T>::add_last(const T& _item)
{
    Node* new_node = new Node(_item);

    if (m_tail == nullptr) {
        m_head = new_node;
        m_tail = new_node;
    } else {
        m_tail->next = new_node;
        m_tail = new_node;
    }

    ++m_size;
}

template <typename T>
bool LinkedList<T>::remove_first()
{
    if (m_head == nullptr)
        return false;

    Node* node = m_head;
    m_head = m_head->next;
    delete node;
    --m_size;

    if (m_head == nullptr)
        m_tail = nullptr;

    return true;
}

template <typename T>
bool LinkedList<T>::remove_item(const T& _item)
{
    Node* current = m_head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->value == _item) {
            if (previous == nullptr) {
                m_head = current->next;
            } else {
                previous->next = current->next;
            }

            if (current == m_tail)
                m_tail = previous;

            delete current;
            --m_size;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

template <typename T>
int LinkedList<T>::search(const T& _item) const
{
    Node* current = m_head;
    int index = 0;

    while (current != nullptr) {
        if (current->value == _item)
            return index;

        current = current->next;
        ++index;
    }

    return -1;
}

template <typename T>
bool LinkedList<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
std::size_t LinkedList<T>::size() const
{
    return m_size;
}

template <typename T>
T& LinkedList<T>::at(std::size_t _index)
{
    Node* current = m_head;
    std::size_t i = 0;

    while (current != nullptr && i < _index) {
        current = current->next;
        ++i;
    }

    return current->value;
}

template <typename T>
const T& LinkedList<T>::at(std::size_t _index) const
{
    const Node* current = m_head;
    std::size_t i = 0;

    while (current != nullptr && i < _index) {
        current = current->next;
        ++i;
    }

    return current->value;
}

#endif // LINKEDLIST_HPP