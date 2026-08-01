#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include <cstddef>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node* prev;

        Node(const T& _value)
            : value(_value), next(nullptr), prev(nullptr)
        {
        }
    };

    Node* m_head;
    Node* m_tail;
    std::size_t m_size;

    void clear();

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    DoublyLinkedList(const DoublyLinkedList& _other);
    DoublyLinkedList& operator=(const DoublyLinkedList& _other);

    void add_first(const T& _item);
    void add_last(const T& _item);
    bool remove_first();
    bool remove_last();
    bool remove_at(std::size_t _index);
    bool remove_item(const T& _item);
    int search(const T& _item) const;

    bool empty() const;
    std::size_t size() const;

    T& at(std::size_t _index);
    const T& at(std::size_t _index) const;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
    : m_head(nullptr), m_tail(nullptr), m_size(0)
{
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    clear();
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& _other)
    : m_head(nullptr), m_tail(nullptr), m_size(0)
{
    Node* current = _other.m_head;
    while (current != nullptr) {
        add_last(current->value);
        current = current->next;
    }
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& _other)
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
void DoublyLinkedList<T>::clear()
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
void DoublyLinkedList<T>::add_first(const T& _item)
{
    Node* new_node = new Node(_item);
    new_node->next = m_head;

    if (m_head != nullptr)
        m_head->prev = new_node;
    else
        m_tail = new_node;

    m_head = new_node;
    ++m_size;
}

template <typename T>
void DoublyLinkedList<T>::add_last(const T& _item)
{
    Node* new_node = new Node(_item);
    new_node->prev = m_tail;

    if (m_tail != nullptr)
        m_tail->next = new_node;
    else
        m_head = new_node;

    m_tail = new_node;
    ++m_size;
}

template <typename T>
bool DoublyLinkedList<T>::remove_first()
{
    if (m_head == nullptr)
        return false;

    Node* node = m_head;
    m_head = m_head->next;

    if (m_head != nullptr)
        m_head->prev = nullptr;
    else
        m_tail = nullptr;

    delete node;
    --m_size;
    return true;
}

template <typename T>
bool DoublyLinkedList<T>::remove_last()
{
    if (m_tail == nullptr)
        return false;

    Node* node = m_tail;
    m_tail = m_tail->prev;

    if (m_tail != nullptr)
        m_tail->next = nullptr;
    else
        m_head = nullptr;

    delete node;
    --m_size;
    return true;
}

template <typename T>
bool DoublyLinkedList<T>::remove_at(std::size_t _index)
{
    if (_index >= m_size)
        return false;

    if (_index == 0)
        return remove_first();

    if (_index == m_size - 1)
        return remove_last();

    Node* current = m_head;
    for (std::size_t i = 0; i < _index; ++i)
        current = current->next;

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    --m_size;
    return true;
}

template <typename T>
bool DoublyLinkedList<T>::remove_item(const T& _item)
{
    Node* current = m_head;
    while (current != nullptr) {
        if (current->value == _item) {
            if (current == m_head)
                return remove_first();

            if (current == m_tail)
                return remove_last();

            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --m_size;
            return true;
        }

        current = current->next;
    }

    return false;
}

template <typename T>
int DoublyLinkedList<T>::search(const T& _item) const
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
bool DoublyLinkedList<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
std::size_t DoublyLinkedList<T>::size() const
{
    return m_size;
}

template <typename T>
T& DoublyLinkedList<T>::at(std::size_t _index)
{
    Node* current = m_head;
    for (std::size_t i = 0; i < _index; ++i)
        current = current->next;

    return current->value;
}

template <typename T>
const T& DoublyLinkedList<T>::at(std::size_t _index) const
{
    Node* current = m_head;
    for (std::size_t i = 0; i < _index; ++i)
        current = current->next;

    return current->value;
}

#endif // DOUBLYLINKEDLIST_HPP