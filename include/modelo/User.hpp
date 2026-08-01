#ifndef USER_HPP
#define USER_HPP

#include <cstddef>
#include <string>

#include <estructuras/Vector.hpp>

class User {
private:
    int m_id;
    std::string m_name;
    std::string m_email;
    std::string m_registration_date;

    Vector<int> m_friend_ids;
    Vector<int> m_post_ids;

    std::size_t m_follower_count;
    std::size_t m_reactions_received;

public:
    User(int _id = 0, const std::string& _name = "", const std::string& _email = "", const std::string& _registration_date = "");

    int id() const;
    const std::string& name() const;
    const std::string& email() const;
    const std::string& registration_date() const;

    void add_friend_id(int _friend_id);
    void remove_friend_id(int _friend_id);
    const Vector<int>& friend_ids() const;
    std::size_t friend_count() const;

    void add_post_id(int _post_id);
    void remove_post_id(int _post_id);
    const Vector<int>& post_ids() const;
    std::size_t post_count() const;

    void set_follower_count(std::size_t _count);
    std::size_t follower_count() const;

    void add_reaction();
    void remove_reactions(std::size_t _count);
    std::size_t reactions_received() const;
};

#endif // USER_HPP
