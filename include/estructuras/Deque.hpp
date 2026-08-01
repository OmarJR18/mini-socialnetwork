#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <cstddef>

template <typename T>
class Deque {
private:
    T* m_data;
    std::size_t m_head;
    std::size_t m_tail;
    std::size_t m_size;
    std::size_t m_capacity;

    void resize(std::size_t _new_capacity);

public:
    explicit Deque(std::size_t _capacity = 4);
    ~Deque();

    Deque(const Deque& _other);
    Deque& operator=(const Deque& _other);

    void add_front(const T& _item);
    void add_back(const T& _item);
    bool remove_front();
    bool remove_back();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    bool empty() const;
    std::size_t size() const;
};

template <typename T>
Deque<T>::Deque(std::size_t _capacity)
    : m_data(nullptr), m_head(0), m_tail(0), m_size(0), m_capacity(0)
{
    if (_capacity == 0)
        _capacity = 4;

    m_capacity = _capacity;
    m_data = new T[m_capacity];
}

template <typename T>
Deque<T>::~Deque()
{
    delete[] m_data;
}

template <typename T>
Deque<T>::Deque(const Deque& _other)
    : m_data(nullptr), m_head(0), m_tail(0), m_size(0), m_capacity(0)
{
    m_capacity = _other.m_capacity;
    m_data = new T[m_capacity];
    m_size = _other.m_size;
    m_head = 0;
    m_tail = m_size;

    for (std::size_t i = 0; i < m_size; ++i)
        m_data[i] = _other.m_data[(_other.m_head + i) % _other.m_capacity];
}

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque& _other)
{
    if (this != &_other) {
        delete[] m_data;
        m_data = nullptr;
        m_head = 0;
        m_tail = 0;
        m_size = 0;
        m_capacity = 0;

        m_capacity = _other.m_capacity;
        m_data = new T[m_capacity];
        m_size = _other.m_size;
        m_head = 0;
        m_tail = m_size;

        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i] = _other.m_data[(_other.m_head + i) % _other.m_capacity];
    }
    return *this;
}

template <typename T>
void Deque<T>::resize(std::size_t _new_capacity)
{
    T* new_data = new T[_new_capacity];

    for (std::size_t i = 0; i < m_size; ++i)
        new_data[i] = m_data[(m_head + i) % m_capacity];

    delete[] m_data;
    m_data = new_data;
    m_capacity = _new_capacity;
    m_head = 0;
    m_tail = m_size;
}

template <typename T>
void Deque<T>::add_front(const T& _item)
{
    if (m_size == m_capacity)
        resize(m_capacity * 2);

    m_head = (m_head + m_capacity - 1) % m_capacity;
    m_data[m_head] = _item;
    ++m_size;
}

template <typename T>
void Deque<T>::add_back(const T& _item)
{
    if (m_size == m_capacity)
        resize(m_capacity * 2);

    m_data[m_tail] = _item;
    m_tail = (m_tail + 1) % m_capacity;
    ++m_size;
}

template <typename T>
bool Deque<T>::remove_front()
{
    if (m_size == 0)
        return false;

    m_head = (m_head + 1) % m_capacity;
    --m_size;
    return true;
}

template <typename T>
bool Deque<T>::remove_back()
{
    if (m_size == 0)
        return false;

    m_tail = (m_tail + m_capacity - 1) % m_capacity;
    --m_size;
    return true;
}

template <typename T>
T& Deque<T>::front()
{
    return m_data[m_head];
}

template <typename T>
const T& Deque<T>::front() const
{
    return m_data[m_head];
}

template <typename T>
T& Deque<T>::back()
{
    return m_data[(m_tail + m_capacity - 1) % m_capacity];
}

template <typename T>
const T& Deque<T>::back() const
{
    return m_data[(m_tail + m_capacity - 1) % m_capacity];
}

template <typename T>
bool Deque<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
std::size_t Deque<T>::size() const
{
    return m_size;
}

#endif // DEQUE_HPP