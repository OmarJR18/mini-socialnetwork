#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <cstddef>

template <typename K>
struct DefaultLess {
    bool operator()(const K& _a, const K& _b) const
    {
        return _a < _b;
    }
};

template <typename K, typename V, typename Compare = DefaultLess<K>>
class AVLTree {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        int height;

        Node(const K& _key, const V& _value)
            : key(_key), value(_value), left(nullptr), right(nullptr), height(1)
        {
        }
    };

    Node* m_root;
    std::size_t m_size;
    Compare m_compare;

    void clear(Node* _node);
    Node* copy(Node* _node);
    int height(Node* _node) const;
    int balance_factor(Node* _node) const;
    void update_height(Node* _node);
    Node* rotate_right(Node* _y);
    Node* rotate_left(Node* _x);
    Node* rebalance(Node* _node);
    Node* insert(Node* _node, const K& _key, const V& _value, bool& _inserted);
    Node* min_node(Node* _node) const;
    Node* remove(Node* _node, const K& _key, bool& _removed);
    Node* search_node(Node* _node, const K& _key) const;

public:
    AVLTree();
    ~AVLTree();

    AVLTree(const AVLTree& _other);
    AVLTree& operator=(const AVLTree& _other);

    bool insert(const K& _key, const V& _value);
    bool remove(const K& _key);

    V* search(const K& _key);
    const V* search(const K& _key) const;

    bool empty() const;
    std::size_t size() const;
    int height() const;
};

template <typename K, typename V, typename Compare>
AVLTree<K, V, Compare>::AVLTree()
    : m_root(nullptr), m_size(0), m_compare()
{
}

template <typename K, typename V, typename Compare>
AVLTree<K, V, Compare>::~AVLTree()
{
    clear(m_root);
}

template <typename K, typename V, typename Compare>
AVLTree<K, V, Compare>::AVLTree(const AVLTree& _other)
    : m_root(nullptr), m_size(_other.m_size), m_compare(_other.m_compare)
{
    m_root = copy(_other.m_root);
}

template <typename K, typename V, typename Compare>
AVLTree<K, V, Compare>& AVLTree<K, V, Compare>::operator=(const AVLTree& _other)
{
    if (this != &_other) {
        clear(m_root);
        m_root = nullptr;
        m_size = _other.m_size;
        m_compare = _other.m_compare;
        m_root = copy(_other.m_root);
    }
    return *this;
}

template <typename K, typename V, typename Compare>
void AVLTree<K, V, Compare>::clear(Node* _node)
{
    if (_node == nullptr)
        return;

    clear(_node->left);
    clear(_node->right);
    delete _node;
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::copy(Node* _node)
{
    if (_node == nullptr)
        return nullptr;

    Node* new_node = new Node(_node->key, _node->value);
    new_node->height = _node->height;
    new_node->left = copy(_node->left);
    new_node->right = copy(_node->right);
    return new_node;
}

template <typename K, typename V, typename Compare>
int AVLTree<K, V, Compare>::height(Node* _node) const
{
    return _node == nullptr ? 0 : _node->height;
}

template <typename K, typename V, typename Compare>
int AVLTree<K, V, Compare>::balance_factor(Node* _node) const
{
    return _node == nullptr ? 0 : height(_node->left) - height(_node->right);
}

template <typename K, typename V, typename Compare>
void AVLTree<K, V, Compare>::update_height(Node* _node)
{
    int left_height = height(_node->left);
    int right_height = height(_node->right);
    _node->height = 1 + (left_height > right_height ? left_height : right_height);
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::rotate_right(Node* _y)
{
    Node* x = _y->left;
    Node* t2 = x->right;

    x->right = _y;
    _y->left = t2;

    update_height(_y);
    update_height(x);
    return x;
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::rotate_left(Node* _x)
{
    Node* y = _x->right;
    Node* t2 = y->left;

    y->left = _x;
    _x->right = t2;

    update_height(_x);
    update_height(y);
    return y;
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::rebalance(Node* _node)
{
    update_height(_node);

    int balance = balance_factor(_node);
    if (balance > 1) {
        if (balance_factor(_node->left) < 0)
            _node->left = rotate_left(_node->left);
        return rotate_right(_node);
    }

    if (balance < -1) {
        if (balance_factor(_node->right) > 0)
            _node->right = rotate_right(_node->right);
        return rotate_left(_node);
    }

    return _node;
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::insert(Node* _node, const K& _key, const V& _value, bool& _inserted)
{
    if (_node == nullptr) {
        _inserted = true;
        return new Node(_key, _value);
    }

    if (m_compare(_key, _node->key)) {
        _node->left = insert(_node->left, _key, _value, _inserted);
    } else if (m_compare(_node->key, _key)) {
        _node->right = insert(_node->right, _key, _value, _inserted);
    } else {
        _node->value = _value;
        return _node;
    }

    return rebalance(_node);
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::min_node(Node* _node) const
{
    Node* current = _node;
    while (current != nullptr && current->left != nullptr)
        current = current->left;
    return current;
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::remove(Node* _node, const K& _key, bool& _removed)
{
    if (_node == nullptr)
        return nullptr;

    if (m_compare(_key, _node->key)) {
        _node->left = remove(_node->left, _key, _removed);
    } else if (m_compare(_node->key, _key)) {
        _node->right = remove(_node->right, _key, _removed);
    } else {
        _removed = true;

        if (_node->left == nullptr || _node->right == nullptr) {
            Node* child = (_node->left != nullptr) ? _node->left : _node->right;
            if (child == nullptr) {
                delete _node;
                return nullptr;
            }

            Node* old_node = _node;
            _node = child;
            delete old_node;
        } else {
            Node* successor = min_node(_node->right);
            _node->key = successor->key;
            _node->value = successor->value;
            _node->right = remove(_node->right, successor->key, _removed);
        }
    }

    if (_node == nullptr)
        return nullptr;

    return rebalance(_node);
}

template <typename K, typename V, typename Compare>
typename AVLTree<K, V, Compare>::Node* AVLTree<K, V, Compare>::search_node(Node* _node, const K& _key) const
{
    if (_node == nullptr)
        return nullptr;

    if (m_compare(_key, _node->key))
        return search_node(_node->left, _key);

    if (m_compare(_node->key, _key))
        return search_node(_node->right, _key);

    return _node;
}

template <typename K, typename V, typename Compare>
bool AVLTree<K, V, Compare>::insert(const K& _key, const V& _value)
{
    bool inserted = false;
    m_root = insert(m_root, _key, _value, inserted);
    if (inserted)
        ++m_size;
    return inserted;
}

template <typename K, typename V, typename Compare>
bool AVLTree<K, V, Compare>::remove(const K& _key)
{
    bool removed = false;
    m_root = remove(m_root, _key, removed);
    if (removed)
        --m_size;
    return removed;
}

template <typename K, typename V, typename Compare>
V* AVLTree<K, V, Compare>::search(const K& _key)
{
    Node* node = search_node(m_root, _key);
    return node == nullptr ? nullptr : &node->value;
}

template <typename K, typename V, typename Compare>
const V* AVLTree<K, V, Compare>::search(const K& _key) const
{
    Node* node = search_node(m_root, _key);
    return node == nullptr ? nullptr : &node->value;
}

template <typename K, typename V, typename Compare>
bool AVLTree<K, V, Compare>::empty() const
{
    return m_size == 0;
}

template <typename K, typename V, typename Compare>
std::size_t AVLTree<K, V, Compare>::size() const
{
    return m_size;
}

template <typename K, typename V, typename Compare>
int AVLTree<K, V, Compare>::height() const
{
    return height(m_root);
}

#endif // AVLTREE_HPP