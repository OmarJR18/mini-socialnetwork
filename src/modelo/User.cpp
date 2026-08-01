#include <modelo/User.hpp>

User::User(int _id, const std::string& _name, const std::string& _email, const std::string& _registration_date)
    : m_id(_id), m_name(_name), m_email(_email), m_registration_date(_registration_date),
      m_friend_ids(), m_post_ids(), m_follower_count(0), m_reactions_received(0)
{
}

int User::id() const
{
    return m_id;
}

const std::string& User::name() const
{
    return m_name;
}

const std::string& User::email() const
{
    return m_email;
}

const std::string& User::registration_date() const
{
    return m_registration_date;
}

void User::add_friend_id(int _friend_id)
{
    if (m_friend_ids.search(_friend_id) == -1)
        m_friend_ids.add_item(_friend_id);
}

void User::remove_friend_id(int _friend_id)
{
    int index = m_friend_ids.search(_friend_id);
    if (index != -1)
        m_friend_ids.remove_index(static_cast<std::size_t>(index));
}

const Vector<int>& User::friend_ids() const
{
    return m_friend_ids;
}

std::size_t User::friend_count() const
{
    return m_friend_ids.size();
}

void User::add_post_id(int _post_id)
{
    m_post_ids.add_item(_post_id);
}

void User::remove_post_id(int _post_id)
{
    int index = m_post_ids.search(_post_id);
    if (index != -1)
        m_post_ids.remove_index(static_cast<std::size_t>(index));
}

const Vector<int>& User::post_ids() const
{
    return m_post_ids;
}

std::size_t User::post_count() const
{
    return m_post_ids.size();
}

void User::set_follower_count(std::size_t _count)
{
    m_follower_count = _count;
}

std::size_t User::follower_count() const
{
    return m_follower_count;
}

void User::add_reaction()
{
    ++m_reactions_received;
}

void User::remove_reactions(std::size_t _count)
{
    m_reactions_received = (_count > m_reactions_received) ? 0 : m_reactions_received - _count;
}

std::size_t User::reactions_received() const
{
    return m_reactions_received;
}
