#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>

#include <estructuras/Deque.hpp>
#include <estructuras/HashTable.hpp>
#include <estructuras/Heap.hpp>
#include <estructuras/LinkedList.hpp>
#include <estructuras/Vector.hpp>

template <typename K, typename Hash = HashDefault<K>, typename Equal = HashEqual<K>>
class Graph {
private:
    struct Suggestion {
        K key;
        std::size_t score;

        Suggestion(const K& _key = K(), std::size_t _score = 0)
            : key(_key), score(_score)
        {
        }
    };

    struct SuggestionCompare {
        bool operator()(const Suggestion& _a, const Suggestion& _b) const
        {
            return _a.score < _b.score;
        }
    };

    Vector<K> m_vertices;
    HashTable<K, LinkedList<K>, Hash, Equal> m_adj;
    Equal m_equal;

    int find_vertex_index(const K& _key) const;
    int find_neighbor_index(const LinkedList<K>& _list, const K& _key) const;
    bool contains_friend(const K& _a, const K& _b) const;
    void ensure_vertex(const K& _key);

public:
    Graph();

    bool add_vertex(const K& _key);
    bool remove_vertex(const K& _key);
    bool add_edge(const K& _a, const K& _b);
    bool remove_edge(const K& _a, const K& _b);

    bool has_vertex(const K& _key) const;
    bool has_edge(const K& _a, const K& _b) const;

    std::size_t vertex_count() const;
    bool empty() const;

    Vector<K> shortest_path(const K& _start, const K& _end) const;
    Vector<K> common_friends(const K& _a, const K& _b) const;
    Vector<K> suggest_friends(const K& _key) const;
};

template <typename K, typename Hash, typename Equal>
Graph<K, Hash, Equal>::Graph()
    : m_vertices(), m_adj(), m_equal()
{
}

template <typename K, typename Hash, typename Equal>
int Graph<K, Hash, Equal>::find_vertex_index(const K& _key) const
{
    for (std::size_t i = 0; i < m_vertices.size(); ++i) {
        if (m_equal(m_vertices[i], _key))
            return static_cast<int>(i);
    }

    return -1;
}

template <typename K, typename Hash, typename Equal>
int Graph<K, Hash, Equal>::find_neighbor_index(const LinkedList<K>& _list, const K& _key) const
{
    for (std::size_t i = 0; i < _list.size(); ++i) {
        if (m_equal(_list.at(i), _key))
            return static_cast<int>(i);
    }

    return -1;
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::contains_friend(const K& _a, const K& _b) const
{
    const LinkedList<K>* list = m_adj.search(_a);
    if (list == nullptr)
        return false;

    return find_neighbor_index(*list, _b) != -1;
}

template <typename K, typename Hash, typename Equal>
void Graph<K, Hash, Equal>::ensure_vertex(const K& _key)
{
    if (!has_vertex(_key)) {
        m_vertices.add_item(_key);
        m_adj.put(_key, LinkedList<K>());
    }
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::add_vertex(const K& _key)
{
    if (has_vertex(_key))
        return false;

    m_vertices.add_item(_key);
    m_adj.put(_key, LinkedList<K>());
    return true;
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::remove_vertex(const K& _key)
{
    int index = find_vertex_index(_key);
    if (index == -1)
        return false;

    for (std::size_t i = 0; i < m_vertices.size(); ++i) {
        if (m_equal(m_vertices[i], _key))
            continue;

        LinkedList<K>* list = m_adj.search(m_vertices[i]);
        if (list != nullptr)
            list->remove_item(_key);
    }

    m_adj.remove(_key);
    m_vertices.remove_index(static_cast<std::size_t>(index));
    return true;
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::add_edge(const K& _a, const K& _b)
{
    ensure_vertex(_a);
    ensure_vertex(_b);

    LinkedList<K>* list_a = m_adj.search(_a);
    LinkedList<K>* list_b = m_adj.search(_b);

    if (list_a == nullptr || list_b == nullptr)
        return false;

    if (find_neighbor_index(*list_a, _b) != -1)
        return false;

    list_a->add_last(_b);
    list_b->add_last(_a);
    return true;
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::remove_edge(const K& _a, const K& _b)
{
    LinkedList<K>* list_a = m_adj.search(_a);
    LinkedList<K>* list_b = m_adj.search(_b);

    if (list_a == nullptr || list_b == nullptr)
        return false;

    int pos_a = find_neighbor_index(*list_a, _b);
    int pos_b = find_neighbor_index(*list_b, _a);

    if (pos_a == -1 || pos_b == -1)
        return false;

    list_a->remove_at(static_cast<std::size_t>(pos_a));
    list_b->remove_at(static_cast<std::size_t>(pos_b));
    return true;
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::has_vertex(const K& _key) const
{
    return m_adj.contains(_key);
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::has_edge(const K& _a, const K& _b) const
{
    return contains_friend(_a, _b);
}
template <typename K, typename Hash, typename Equal>
std::size_t Graph<K, Hash, Equal>::vertex_count() const
{
    return m_vertices.size();
}

template <typename K, typename Hash, typename Equal>
bool Graph<K, Hash, Equal>::empty() const
{
    return m_vertices.empty();
}

template <typename K, typename Hash, typename Equal>
Vector<K> Graph<K, Hash, Equal>::shortest_path(const K& _start, const K& _end) const
{
    Vector<K> path;
    if (!has_vertex(_start) || !has_vertex(_end))
        return path;

    if (m_equal(_start, _end)) {
        path.add_item(_start);
        return path;
    }

    HashTable<K, bool, Hash, Equal> visited(m_vertices.size() * 2 + 1);
    HashTable<K, K, Hash, Equal> parent(m_vertices.size() * 2 + 1);
    Deque<K> queue;

    visited.put(_start, true);
    queue.add_back(_start);

    bool found = false;
    while (!queue.empty()) {
        K current = queue.front();
        queue.remove_front();

        const LinkedList<K>* neighbors = m_adj.search(current);
        if (neighbors == nullptr)
            continue;

        for (std::size_t i = 0; i < neighbors->size(); ++i) {
            const K& next = neighbors->at(i);
            if (visited.contains(next))
                continue;

            visited.put(next, true);
            parent.put(next, current);
            if (m_equal(next, _end)) {
                found = true;
                queue = Deque<K>();
                break;
            }

            queue.add_back(next);
        }
    }

    if (!found)
        return path;

    Vector<K> reversed;
    K current = _end;
    reversed.add_item(current);

    while (!m_equal(current, _start)) {
        const K* prev = parent.search(current);
        if (prev == nullptr)
            return Vector<K>();

        current = *prev;
        reversed.add_item(current);
    }

    for (std::size_t i = 0; i < reversed.size(); ++i)
        path.add_item(reversed[reversed.size() - 1 - i]);

    return path;
}

template <typename K, typename Hash, typename Equal>
Vector<K> Graph<K, Hash, Equal>::common_friends(const K& _a, const K& _b) const
{
    Vector<K> result;
    const LinkedList<K>* list_a = m_adj.search(_a);
    const LinkedList<K>* list_b = m_adj.search(_b);

    if (list_a == nullptr || list_b == nullptr)
        return result;

    for (std::size_t i = 0; i < list_a->size(); ++i) {
        const K& candidate = list_a->at(i);
        if (find_neighbor_index(*list_b, candidate) == -1)
            continue;

        bool exists = false;
        for (std::size_t j = 0; j < result.size(); ++j) {
            if (m_equal(result[j], candidate)) {
                exists = true;
                break;
            }
        }

        if (!exists)
            result.add_item(candidate);
    }

    return result;
}

template <typename K, typename Hash, typename Equal>
Vector<K> Graph<K, Hash, Equal>::suggest_friends(const K& _key) const
{
    Vector<K> result;
    const LinkedList<K>* list = m_adj.search(_key);
    if (list == nullptr)
        return result;

    Heap<Suggestion, SuggestionCompare> ranking;

    for (std::size_t i = 0; i < m_vertices.size(); ++i) {
        const K& candidate = m_vertices[i];
        if (m_equal(candidate, _key) || contains_friend(_key, candidate))
            continue;

        const LinkedList<K>* candidate_list = m_adj.search(candidate);
        if (candidate_list == nullptr)
            continue;

        std::size_t score = 0;
        for (std::size_t j = 0; j < candidate_list->size(); ++j) {
            if (find_neighbor_index(*list, candidate_list->at(j)) != -1)
                ++score;
        }

        if (score > 0)
            ranking.insert(Suggestion(candidate, score));
    }

    Suggestion item;
    while (ranking.extract_top(item))
        result.add_item(item.key);

    return result;
}

#endif // GRAPH_HPP