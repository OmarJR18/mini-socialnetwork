#ifndef POST_HPP
#define POST_HPP

#include <cstddef>
#include <string>

#include <estructuras/Vector.hpp>

class Post {
private:
    int m_id;
    int m_owner_id;
    std::string m_date;
    std::string m_text;
    std::size_t m_like_count;
    Vector<int> m_comment_ids;

public:
    Post(int _id = 0, int _owner_id = 0, const std::string& _date = "", const std::string& _text = "");

    int id() const;
    int owner_id() const;
    const std::string& date() const;
    const std::string& text() const;

    void add_like();
    std::size_t like_count() const;

    void add_comment_id(int _comment_id);
    const Vector<int>& comment_ids() const;
    std::size_t comment_count() const;
};

#endif // POST_HPP
