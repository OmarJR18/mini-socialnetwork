#ifndef SOCIALNETWORK_HPP
#define SOCIALNETWORK_HPP

#include <cstddef>
#include <string>

#include <estructuras/AVLTree.hpp>
#include <estructuras/Graph.hpp>
#include <estructuras/HashTable.hpp>
#include <estructuras/LinkedList.hpp>
#include <estructuras/Vector.hpp>

#include <modelo/Comment.hpp>
#include <modelo/Post.hpp>
#include <modelo/User.hpp>

// Servicio orquestador: conecta el indice de usuarios (HashTable), el indice
// secundario por nombre (AVLTree), el grafo de amistades (Graph) y expone
// las operaciones que pide el enunciado.
class SocialNetwork {
private:
    HashTable<int, User> m_users;
    HashTable<int, Post> m_posts;
    HashTable<int, Comment> m_comments;
    AVLTree<std::string, LinkedList<int>> m_users_by_name;
    Graph<int> m_friendships;

    // registros de ids activos, usados para poder recorrer todos los
    // usuarios/publicaciones (HashTable no permite iterar sus entradas)
    Vector<int> m_user_ids;
    Vector<int> m_post_ids;

    int m_next_user_id;
    int m_next_post_id;
    int m_next_comment_id;

    void index_name(const std::string& _name, int _user_id);
    void unindex_name(const std::string& _name, int _user_id);

public:
    SocialNetwork();

    int register_user(const std::string& _name, const std::string& _email, const std::string& _registration_date);
    bool remove_user(int _user_id);
    User* find_user(int _user_id);
    const User* find_user(int _user_id) const;
    Vector<int> search_users_by_name(const std::string& _name) const;

    int create_post(int _owner_id, const std::string& _text, const std::string& _date);
    bool remove_post(int _post_id);
    Post* find_post(int _post_id);
    const Post* find_post(int _post_id) const;
    Vector<int> posts_of_user(int _user_id) const;

    bool react_to_post(int _post_id);
    int comment_on_post(int _post_id, int _author_id, const std::string& _text, const std::string& _date);
    Vector<Comment> comments_of_post(int _post_id) const;

    bool add_friend(int _user_a, int _user_b);
    bool remove_friend(int _user_a, int _user_b);
    Vector<int> friendship_path(int _user_a, int _user_b) const;
    Vector<int> common_friends(int _user_a, int _user_b) const;
    Vector<int> friend_suggestions(int _user_id) const;

    Vector<int> most_active_users(std::size_t _top_k) const;
    Vector<int> most_reacted_posts(std::size_t _top_k) const;

    std::size_t user_count() const;
    std::size_t post_count() const;
};

#endif // SOCIALNETWORK_HPP
