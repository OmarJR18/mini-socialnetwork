#include <modelo/Comment.hpp>

Comment::Comment(int _id, int _author_id, int _post_id, const std::string& _text, const std::string& _date)
    : m_id(_id), m_author_id(_author_id), m_post_id(_post_id), m_text(_text), m_date(_date)
{
}

int Comment::id() const
{
    return m_id;
}

int Comment::author_id() const
{
    return m_author_id;
}

int Comment::post_id() const
{
    return m_post_id;
}

const std::string& Comment::text() const
{
    return m_text;
}

const std::string& Comment::date() const
{
    return m_date;
}
