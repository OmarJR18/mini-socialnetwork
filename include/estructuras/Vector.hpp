#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

template <typename T>
class Vector {
private:
    T* m_data;
    std::size_t m_size;
    std::size_t m_capacity;

public:
    explicit Vector(std::size_t _capacity = 0);
    ~Vector();

    Vector(const Vector& _other);
    Vector& operator=(const Vector& _other);

    void add_item(const T& _item);
    void remove_index(std::size_t _index);
    int search(const T& _item) const;

    std::size_t size() const;
    std::size_t capacity() const;

    const T& operator[](std::size_t _index) const;
    T& operator[](std::size_t _index);
};

template <typename T>
Vector<T>::Vector(std::size_t _capacity)
    : m_data(nullptr), m_size(0), m_capacity(0)
{
    if (_capacity > 0) {
        m_capacity = _capacity;
        m_data = new T[m_capacity];
    }
}

template <typename T>
Vector<T>::~Vector()
{
    delete[] m_data;
}

template <typename T>
Vector<T>::Vector(const Vector& _other)
    : m_data(nullptr), m_size(0), m_capacity(0)
{
    if (_other.m_capacity > 0) {
        m_capacity = _other.m_capacity;
        m_data = new T[m_capacity];
        for (std::size_t i = 0; i < _other.m_size; ++i)
            m_data[i] = _other.m_data[i];
        m_size = _other.m_size;
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& _other)
{
    if (this != &_other) {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;

        if (_other.m_capacity > 0) {
            m_capacity = _other.m_capacity;
            m_data = new T[m_capacity];
            for (std::size_t i = 0; i < _other.m_size; ++i)
                m_data[i] = _other.m_data[i];
            m_size = _other.m_size;
        }
    }
    return *this;
}

template <typename T>
void Vector<T>::add_item(const T& _item)
{
    if (m_size >= m_capacity) {
        // duplica la capacidad para mantener inserción amortizada O(1)
        std::size_t new_cap = (m_capacity == 0) ? 1 : m_capacity * 2;
        T* new_data = new T[new_cap];
        for (std::size_t i = 0; i < m_size; ++i)
            new_data[i] = m_data[i];
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_cap;
    }
    m_data[m_size] = _item;
    ++m_size;
}

template <typename T>
void Vector<T>::remove_index(std::size_t _index)
{
    if (_index >= m_size)
        return;
    m_data[_index] = m_data[m_size - 1];
    --m_size;
}

template <typename T>
int Vector<T>::search(const T& _item) const
{
    for (std::size_t i = 0; i < m_size; ++i)
        if (m_data[i] == _item)
            return static_cast<int>(i);
    return -1;
}

template <typename T>
std::size_t Vector<T>::size() const
{
    return m_size;
}

template <typename T>
std::size_t Vector<T>::capacity() const
{
    return m_capacity;
}

template <typename T>
const T& Vector<T>::operator[](std::size_t _index) const
{
    return m_data[_index];
}

template <typename T>
T& Vector<T>::operator[](std::size_t _index)
{
    return m_data[_index];
}

#endif // VECTOR_HPP