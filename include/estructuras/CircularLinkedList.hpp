#ifndef CIRCULARLINKEDLIST_HPP
#define CIRCULARLINKEDLIST_HPP

#include <cstddef>

template <typename T>
class CircularLinkedList {
private:
    struct Node {
        T value;
        Node* next;

        Node(const T& _value)
            : value(_value), next(nullptr)
        {
        }
    };

    Node* m_tail;
    std::size_t m_size;

    void clear();

public:
    CircularLinkedList();
    ~CircularLinkedList();

    CircularLinkedList(const CircularLinkedList& _other);
    CircularLinkedList& operator=(const CircularLinkedList& _other);

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
CircularLinkedList<T>::CircularLinkedList()
    : m_tail(nullptr), m_size(0)
{
}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList()
{
    clear();
}

template <typename T>
CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList& _other)
    : m_tail(nullptr), m_size(0)
{
    for (std::size_t i = 0; i < _other.m_size; ++i)
        add_last(_other.at(i));
}

template <typename T>
CircularLinkedList<T>& CircularLinkedList<T>::operator=(const CircularLinkedList& _other)
{
    if (this != &_other) {
        clear();
        for (std::size_t i = 0; i < _other.m_size; ++i)
            add_last(_other.at(i));
    }
    return *this;
}

template <typename T>
void CircularLinkedList<T>::clear()
{
    while (m_size > 0)
        remove_first();
}

template <typename T>
void CircularLinkedList<T>::add_first(const T& _item)
{
    Node* new_node = new Node(_item);

    if (m_tail == nullptr) {
        new_node->next = new_node;
        m_tail = new_node;
    } else {
        new_node->next = m_tail->next;
        m_tail->next = new_node;
    }

    ++m_size;
}

template <typename T>
void CircularLinkedList<T>::add_last(const T& _item)
{
    Node* new_node = new Node(_item);

    if (m_tail == nullptr) {
        new_node->next = new_node;
        m_tail = new_node;
    } else {
        new_node->next = m_tail->next;
        m_tail->next = new_node;
        m_tail = new_node;
    }

    ++m_size;
}

template <typename T>
bool CircularLinkedList<T>::remove_first()
{
    if (m_tail == nullptr)
        return false;

    Node* head = m_tail->next;

    if (head == m_tail) {
        delete head;
        m_tail = nullptr;
    } else {
        m_tail->next = head->next;
        delete head;
    }

    --m_size;
    return true;
}

template <typename T>
bool CircularLinkedList<T>::remove_last()
{
    if (m_tail == nullptr)
        return false;

    if (m_tail->next == m_tail)
        return remove_first();

    Node* current = m_tail->next;
    while (current->next != m_tail)
        current = current->next;

    Node* node = m_tail;
    current->next = m_tail->next;
    m_tail = current;
    delete node;
    --m_size;
    return true;
}

template <typename T>
bool CircularLinkedList<T>::remove_at(std::size_t _index)
{
    if (_index >= m_size)
        return false;

    if (_index == 0)
        return remove_first();

    if (_index == m_size - 1)
        return remove_last();

    Node* current = m_tail->next;
    for (std::size_t i = 0; i < _index - 1; ++i)
        current = current->next;

    Node* target = current->next;
    current->next = target->next;
    delete target;
    --m_size;
    return true;
}

template <typename T>
bool CircularLinkedList<T>::remove_item(const T& _item)
{
    if (m_tail == nullptr)
        return false;

    Node* previous = m_tail;
    Node* current = m_tail->next;

    for (std::size_t i = 0; i < m_size; ++i) {
        if (current->value == _item) {
            if (current == m_tail->next)
                return remove_first();

            if (current == m_tail)
                return remove_last();

            previous->next = current->next;
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
int CircularLinkedList<T>::search(const T& _item) const
{
    if (m_tail == nullptr)
        return -1;

    Node* current = m_tail->next;
    for (std::size_t i = 0; i < m_size; ++i) {
        if (current->value == _item)
            return static_cast<int>(i);

        current = current->next;
    }

    return -1;
}

template <typename T>
bool CircularLinkedList<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
std::size_t CircularLinkedList<T>::size() const
{
    return m_size;
}

template <typename T>
T& CircularLinkedList<T>::at(std::size_t _index)
{
    Node* current = m_tail->next;
    for (std::size_t i = 0; i < _index; ++i)
        current = current->next;

    return current->value;
}

template <typename T>
const T& CircularLinkedList<T>::at(std::size_t _index) const
{
    Node* current = m_tail->next;
    for (std::size_t i = 0; i < _index; ++i)
        current = current->next;

    return current->value;
}

#endif // CIRCULARLINKEDLIST_HPP