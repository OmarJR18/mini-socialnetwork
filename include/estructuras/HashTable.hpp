#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <functional>
#include <string>

#include <estructuras/LinkedList.hpp>

template <typename K>
struct HashDefault {
    std::size_t operator()(const K& _key) const
    {
        return std::hash<K>{}(_key);
    }
};

template <>
struct HashDefault<const char*> {
    std::size_t operator()(const char* _key) const
    {
        return std::hash<std::string>{}(std::string(_key));
    }
};

template <typename K>
struct HashEqual {
    bool operator()(const K& _a, const K& _b) const
    {
        return _a == _b;
    }
};

template <typename K, typename V, typename Hash = HashDefault<K>, typename Equal = HashEqual<K>>
class HashTable {
private:
    struct Entry {
        K key;
        V value;

        Entry(const K& _key = K(), const V& _value = V())
            : key(_key), value(_value)
        {
        }

    };

    LinkedList<Entry>* m_buckets;
    std::size_t m_size;
    std::size_t m_capacity;
    float m_max_load;
    Hash m_hash;
    Equal m_equal;

    std::size_t index_for(const K& _key) const;
    int find_index(std::size_t _bucket, const K& _key) const;
    void rehash(std::size_t _new_capacity);
    void clear();

public:
    explicit HashTable(std::size_t _capacity = 8, float _max_load = 0.75f);
    ~HashTable();

    HashTable(const HashTable& _other);
    HashTable& operator=(const HashTable& _other);

    bool put(const K& _key, const V& _value);
    bool remove(const K& _key);

    V* search(const K& _key);
    const V* search(const K& _key) const;

    bool contains(const K& _key) const;
    bool empty() const;
    std::size_t size() const;
    std::size_t capacity() const;
};

template <typename K, typename V, typename Hash, typename Equal>
HashTable<K, V, Hash, Equal>::HashTable(std::size_t _capacity, float _max_load)
    : m_buckets(nullptr), m_size(0), m_capacity(0), m_max_load(_max_load), m_hash(), m_equal()
{
    if (_capacity == 0)
        _capacity = 8;

    m_capacity = _capacity;
    m_buckets = new LinkedList<Entry>[m_capacity];
}

template <typename K, typename V, typename Hash, typename Equal>
HashTable<K, V, Hash, Equal>::~HashTable()
{
    clear();
}

template <typename K, typename V, typename Hash, typename Equal>
HashTable<K, V, Hash, Equal>::HashTable(const HashTable& _other)
    : m_buckets(nullptr), m_size(_other.m_size), m_capacity(_other.m_capacity), m_max_load(_other.m_max_load), m_hash(_other.m_hash), m_equal(_other.m_equal)
{
    m_buckets = new LinkedList<Entry>[m_capacity];
    for (std::size_t i = 0; i < m_capacity; ++i)
        m_buckets[i] = _other.m_buckets[i];
}

template <typename K, typename V, typename Hash, typename Equal>
HashTable<K, V, Hash, Equal>& HashTable<K, V, Hash, Equal>::operator=(const HashTable& _other)
{
    if (this != &_other) {
        clear();
        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
        m_max_load = _other.m_max_load;
        m_hash = _other.m_hash;
        m_equal = _other.m_equal;
        m_buckets = new LinkedList<Entry>[m_capacity];
        for (std::size_t i = 0; i < m_capacity; ++i)
            m_buckets[i] = _other.m_buckets[i];
    }
    return *this;
}

template <typename K, typename V, typename Hash, typename Equal>
std::size_t HashTable<K, V, Hash, Equal>::index_for(const K& _key) const
{
    return m_hash(_key) % m_capacity;
}

template <typename K, typename V, typename Hash, typename Equal>
int HashTable<K, V, Hash, Equal>::find_index(std::size_t _bucket, const K& _key) const
{
    for (std::size_t i = 0; i < m_buckets[_bucket].size(); ++i) {
        if (m_equal(m_buckets[_bucket].at(i).key, _key))
            return static_cast<int>(i);
    }

    return -1;
}

template <typename K, typename V, typename Hash, typename Equal>
void HashTable<K, V, Hash, Equal>::rehash(std::size_t _new_capacity)
{
    LinkedList<Entry>* new_buckets = new LinkedList<Entry>[_new_capacity];

    for (std::size_t i = 0; i < m_capacity; ++i) {
        for (std::size_t j = 0; j < m_buckets[i].size(); ++j) {
            const Entry& entry = m_buckets[i].at(j);
            std::size_t new_index = m_hash(entry.key) % _new_capacity;
            new_buckets[new_index].add_last(entry);
        }
    }

    delete[] m_buckets;
    m_buckets = new_buckets;
    m_capacity = _new_capacity;
}

template <typename K, typename V, typename Hash, typename Equal>
void HashTable<K, V, Hash, Equal>::clear()
{
    delete[] m_buckets;
    m_buckets = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template <typename K, typename V, typename Hash, typename Equal>
bool HashTable<K, V, Hash, Equal>::put(const K& _key, const V& _value)
{
    if (m_buckets == nullptr)
        return false;

    std::size_t idx = index_for(_key);
    int pos = find_index(idx, _key);

    if (pos != -1) {
        m_buckets[idx].at(static_cast<std::size_t>(pos)).value = _value;
        return false;
    }

    m_buckets[idx].add_last(Entry(_key, _value));
    ++m_size;

    if (static_cast<float>(m_size) / static_cast<float>(m_capacity) > m_max_load)
        rehash(m_capacity * 2);

    return true;
}

template <typename K, typename V, typename Hash, typename Equal>
bool HashTable<K, V, Hash, Equal>::remove(const K& _key)
{
    if (m_buckets == nullptr)
        return false;

    std::size_t idx = index_for(_key);
    int pos = find_index(idx, _key);
    if (pos == -1)
        return false;

    m_buckets[idx].remove_at(static_cast<std::size_t>(pos));
    --m_size;
    return true;
}

template <typename K, typename V, typename Hash, typename Equal>
V* HashTable<K, V, Hash, Equal>::search(const K& _key)
{
    if (m_buckets == nullptr)
        return nullptr;

    std::size_t idx = index_for(_key);
    int pos = find_index(idx, _key);
    if (pos == -1)
        return nullptr;

    return &m_buckets[idx].at(static_cast<std::size_t>(pos)).value;
}

template <typename K, typename V, typename Hash, typename Equal>
const V* HashTable<K, V, Hash, Equal>::search(const K& _key) const
{
    if (m_buckets == nullptr)
        return nullptr;

    std::size_t idx = index_for(_key);
    int pos = find_index(idx, _key);
    if (pos == -1)
        return nullptr;

    return &m_buckets[idx].at(static_cast<std::size_t>(pos)).value;
}

template <typename K, typename V, typename Hash, typename Equal>
bool HashTable<K, V, Hash, Equal>::contains(const K& _key) const
{
    return search(_key) != nullptr;
}

template <typename K, typename V, typename Hash, typename Equal>
bool HashTable<K, V, Hash, Equal>::empty() const
{
    return m_size == 0;
}

template <typename K, typename V, typename Hash, typename Equal>
std::size_t HashTable<K, V, Hash, Equal>::size() const
{
    return m_size;
}

template <typename K, typename V, typename Hash, typename Equal>
std::size_t HashTable<K, V, Hash, Equal>::capacity() const
{
    return m_capacity;
}

#endif // HASHTABLE_HPP