#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstddef>

template <typename T>
struct HeapLess {
    bool operator()(const T& _a, const T& _b) const
    {
        return _a < _b;
    }
};

template <typename T, typename Compare = HeapLess<T>>
class Heap {
private:
    T* m_data;
    std::size_t m_size;
    std::size_t m_capacity;
    Compare m_compare;

    void resize(std::size_t _new_capacity);
    void sift_up(std::size_t _index);
    void sift_down(std::size_t _index);

public:
    explicit Heap(std::size_t _capacity = 4);
    ~Heap();

    Heap(const Heap& _other);
    Heap& operator=(const Heap& _other);

    void insert(const T& _item);
    bool extract_top(T& _out);

    T& top();
    const T& top() const;

    bool empty() const;
    std::size_t size() const;
};

template <typename T, typename Compare>
Heap<T, Compare>::Heap(std::size_t _capacity)
    : m_data(nullptr), m_size(0), m_capacity(0), m_compare()
{
    if (_capacity == 0)
        _capacity = 4;

    m_capacity = _capacity;
    m_data = new T[m_capacity];
}

template <typename T, typename Compare>
Heap<T, Compare>::~Heap()
{
    delete[] m_data;
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Heap& _other)
    : m_data(nullptr), m_size(_other.m_size), m_capacity(_other.m_capacity), m_compare(_other.m_compare)
{
    m_data = new T[m_capacity];
    for (std::size_t i = 0; i < m_size; ++i)
        m_data[i] = _other.m_data[i];
}

template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(const Heap& _other)
{
    if (this != &_other) {
        delete[] m_data;
        m_data = nullptr;
        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
        m_compare = _other.m_compare;
        m_data = new T[m_capacity];

        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i] = _other.m_data[i];
    }
    return *this;
}

template <typename T, typename Compare>
void Heap<T, Compare>::resize(std::size_t _new_capacity)
{
    T* new_data = new T[_new_capacity];
    for (std::size_t i = 0; i < m_size; ++i)
        new_data[i] = m_data[i];

    delete[] m_data;
    m_data = new_data;
    m_capacity = _new_capacity;
}

template <typename T, typename Compare>
void Heap<T, Compare>::sift_up(std::size_t _index)
{
    while (_index > 0) {
        std::size_t parent = (_index - 1) / 2;
        if (!m_compare(m_data[parent], m_data[_index]))
            break;

        T tmp = m_data[parent];
        m_data[parent] = m_data[_index];
        m_data[_index] = tmp;
        _index = parent;
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::sift_down(std::size_t _index)
{
    while (true) {
        std::size_t left = _index * 2 + 1;
        std::size_t right = _index * 2 + 2;
        std::size_t best = _index;

        if (left < m_size && m_compare(m_data[best], m_data[left]))
            best = left;

        if (right < m_size && m_compare(m_data[best], m_data[right]))
            best = right;

        if (best == _index)
            break;

        T tmp = m_data[_index];
        m_data[_index] = m_data[best];
        m_data[best] = tmp;
        _index = best;
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::insert(const T& _item)
{
    if (m_size == m_capacity)
        resize(m_capacity * 2);

    m_data[m_size] = _item;
    sift_up(m_size);
    ++m_size;
}

template <typename T, typename Compare>
bool Heap<T, Compare>::extract_top(T& _out)
{
    if (m_size == 0)
        return false;

    _out = m_data[0];
    --m_size;

    if (m_size > 0) {
        m_data[0] = m_data[m_size];
        sift_down(0);
    }

    return true;
}

template <typename T, typename Compare>
T& Heap<T, Compare>::top()
{
    return m_data[0];
}

template <typename T, typename Compare>
const T& Heap<T, Compare>::top() const
{
    return m_data[0];
}

template <typename T, typename Compare>
bool Heap<T, Compare>::empty() const
{
    return m_size == 0;
}

template <typename T, typename Compare>
std::size_t Heap<T, Compare>::size() const
{
    return m_size;
}

#endif // HEAP_HPP