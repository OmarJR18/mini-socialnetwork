#include <modelo/Post.hpp>

Post::Post(int _id, int _owner_id, const std::string& _date, const std::string& _text)
    : m_id(_id), m_owner_id(_owner_id), m_date(_date), m_text(_text),
      m_like_count(0), m_comment_ids()
{
}

int Post::id() const
{
    return m_id;
}

int Post::owner_id() const
{
    return m_owner_id;
}

const std::string& Post::date() const
{
    return m_date;
}

const std::string& Post::text() const
{
    return m_text;
}

void Post::add_like()
{
    ++m_like_count;
}

std::size_t Post::like_count() const
{
    return m_like_count;
}

void Post::add_comment_id(int _comment_id)
{
    m_comment_ids.add_item(_comment_id);
}

const Vector<int>& Post::comment_ids() const
{
    return m_comment_ids;
}

std::size_t Post::comment_count() const
{
    return m_comment_ids.size();
}
