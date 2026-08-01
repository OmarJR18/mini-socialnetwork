#include <servicios/SocialNetwork.hpp>

#include <estructuras/Heap.hpp>

namespace {

struct RankedId {
    int id;
    std::size_t score;

    RankedId(int _id = 0, std::size_t _score = 0)
        : id(_id), score(_score)
    {
    }
};

struct RankedCompare {
    bool operator()(const RankedId& _a, const RankedId& _b) const
    {
        return _a.score < _b.score;
    }
};

} // namespace

SocialNetwork::SocialNetwork()
    : m_users(), m_posts(), m_comments(), m_users_by_name(), m_friendships(),
      m_user_ids(), m_post_ids(), m_next_user_id(1), m_next_post_id(1), m_next_comment_id(1)
{
}

void SocialNetwork::index_name(const std::string& _name, int _user_id)
{
    LinkedList<int>* bucket = m_users_by_name.search(_name);
    if (bucket == nullptr) {
        m_users_by_name.insert(_name, LinkedList<int>());
        bucket = m_users_by_name.search(_name);
    }

    bucket->add_last(_user_id);
}

void SocialNetwork::unindex_name(const std::string& _name, int _user_id)
{
    LinkedList<int>* bucket = m_users_by_name.search(_name);
    if (bucket == nullptr)
        return;

    bucket->remove_item(_user_id);
    if (bucket->empty())
        m_users_by_name.remove(_name);
}

int SocialNetwork::register_user(const std::string& _name, const std::string& _email, const std::string& _registration_date)
{
    int id = m_next_user_id++;
    m_users.put(id, User(id, _name, _email, _registration_date));
    m_user_ids.add_item(id);
    m_friendships.add_vertex(id);
    index_name(_name, id);
    return id;
}

bool SocialNetwork::remove_user(int _user_id)
{
    User* user = m_users.search(_user_id);
    if (user == nullptr)
        return false;

    // se copian antes de iterar: remove_friend/remove_post modifican los
    // vectores originales del propio usuario mientras recorremos
    Vector<int> friend_ids = user->friend_ids();
    for (std::size_t i = 0; i < friend_ids.size(); ++i)
        remove_friend(_user_id, friend_ids[i]);

    Vector<int> post_ids = user->post_ids();
    for (std::size_t i = 0; i < post_ids.size(); ++i)
        remove_post(post_ids[i]);

    unindex_name(user->name(), _user_id);
    m_friendships.remove_vertex(_user_id);
    m_users.remove(_user_id);

    int index = m_user_ids.search(_user_id);
    if (index != -1)
        m_user_ids.remove_index(static_cast<std::size_t>(index));

    return true;
}

User* SocialNetwork::find_user(int _user_id)
{
    return m_users.search(_user_id);
}

const User* SocialNetwork::find_user(int _user_id) const
{
    return m_users.search(_user_id);
}

Vector<int> SocialNetwork::search_users_by_name(const std::string& _name) const
{
    Vector<int> result;
    const LinkedList<int>* bucket = m_users_by_name.search(_name);
    if (bucket == nullptr)
        return result;

    for (std::size_t i = 0; i < bucket->size(); ++i)
        result.add_item(bucket->at(i));

    return result;
}

int SocialNetwork::create_post(int _owner_id, const std::string& _text, const std::string& _date)
{
    User* owner = m_users.search(_owner_id);
    if (owner == nullptr)
        return -1;

    int id = m_next_post_id++;
    m_posts.put(id, Post(id, _owner_id, _date, _text));
    m_post_ids.add_item(id);
    owner->add_post_id(id);
    return id;
}

bool SocialNetwork::remove_post(int _post_id)
{
    Post* post = m_posts.search(_post_id);
    if (post == nullptr)
        return false;

    const Vector<int>& comment_ids = post->comment_ids();
    for (std::size_t i = 0; i < comment_ids.size(); ++i)
        m_comments.remove(comment_ids[i]);

    User* owner = m_users.search(post->owner_id());
    if (owner != nullptr) {
        owner->remove_post_id(_post_id);
        owner->remove_reactions(post->like_count());
    }

    m_posts.remove(_post_id);

    int index = m_post_ids.search(_post_id);
    if (index != -1)
        m_post_ids.remove_index(static_cast<std::size_t>(index));

    return true;
}

Post* SocialNetwork::find_post(int _post_id)
{
    return m_posts.search(_post_id);
}

const Post* SocialNetwork::find_post(int _post_id) const
{
    return m_posts.search(_post_id);
}

Vector<int> SocialNetwork::posts_of_user(int _user_id) const
{
    const User* user = m_users.search(_user_id);
    if (user == nullptr)
        return Vector<int>();

    return user->post_ids();
}

bool SocialNetwork::react_to_post(int _post_id)
{
    Post* post = m_posts.search(_post_id);
    if (post == nullptr)
        return false;

    post->add_like();

    User* owner = m_users.search(post->owner_id());
    if (owner != nullptr)
        owner->add_reaction();

    return true;
}

int SocialNetwork::comment_on_post(int _post_id, int _author_id, const std::string& _text, const std::string& _date)
{
    Post* post = m_posts.search(_post_id);
    if (post == nullptr || m_users.search(_author_id) == nullptr)
        return -1;

    int id = m_next_comment_id++;
    m_comments.put(id, Comment(id, _author_id, _post_id, _text, _date));
    post->add_comment_id(id);
    return id;
}

Vector<Comment> SocialNetwork::comments_of_post(int _post_id) const
{
    Vector<Comment> result;
    const Post* post = m_posts.search(_post_id);
    if (post == nullptr)
        return result;

    const Vector<int>& comment_ids = post->comment_ids();
    for (std::size_t i = 0; i < comment_ids.size(); ++i) {
        const Comment* comment = m_comments.search(comment_ids[i]);
        if (comment != nullptr)
            result.add_item(*comment);
    }

    return result;
}

bool SocialNetwork::add_friend(int _user_a, int _user_b)
{
    User* user_a = m_users.search(_user_a);
    User* user_b = m_users.search(_user_b);
    if (user_a == nullptr || user_b == nullptr)
        return false;

    if (!m_friendships.add_edge(_user_a, _user_b))
        return false;

    user_a->add_friend_id(_user_b);
    user_b->add_friend_id(_user_a);
    return true;
}

bool SocialNetwork::remove_friend(int _user_a, int _user_b)
{
    if (!m_friendships.remove_edge(_user_a, _user_b))
        return false;

    User* user_a = m_users.search(_user_a);
    User* user_b = m_users.search(_user_b);
    if (user_a != nullptr)
        user_a->remove_friend_id(_user_b);
    if (user_b != nullptr)
        user_b->remove_friend_id(_user_a);

    return true;
}

Vector<int> SocialNetwork::friendship_path(int _user_a, int _user_b) const
{
    return m_friendships.shortest_path(_user_a, _user_b);
}

Vector<int> SocialNetwork::common_friends(int _user_a, int _user_b) const
{
    return m_friendships.common_friends(_user_a, _user_b);
}

Vector<int> SocialNetwork::friend_suggestions(int _user_id) const
{
    return m_friendships.suggest_friends(_user_id);
}

Vector<int> SocialNetwork::most_active_users(std::size_t _top_k) const
{
    Vector<int> result;
    if (m_user_ids.size() == 0)
        return result;

    Heap<RankedId, RankedCompare> ranking(m_user_ids.size());
    for (std::size_t i = 0; i < m_user_ids.size(); ++i) {
        const User* user = m_users.search(m_user_ids[i]);
        if (user != nullptr)
            ranking.insert(RankedId(user->id(), user->post_count()));
    }

    std::size_t limit = (_top_k < ranking.size()) ? _top_k : ranking.size();
    for (std::size_t i = 0; i < limit; ++i) {
        RankedId top;
        ranking.extract_top(top);
        result.add_item(top.id);
    }

    return result;
}

Vector<int> SocialNetwork::most_reacted_posts(std::size_t _top_k) const
{
    Vector<int> result;
    if (m_post_ids.size() == 0)
        return result;

    Heap<RankedId, RankedCompare> ranking(m_post_ids.size());
    for (std::size_t i = 0; i < m_post_ids.size(); ++i) {
        const Post* post = m_posts.search(m_post_ids[i]);
        if (post != nullptr)
            ranking.insert(RankedId(post->id(), post->like_count()));
    }

    std::size_t limit = (_top_k < ranking.size()) ? _top_k : ranking.size();
    for (std::size_t i = 0; i < limit; ++i) {
        RankedId top;
        ranking.extract_top(top);
        result.add_item(top.id);
    }

    return result;
}

std::size_t SocialNetwork::user_count() const
{
    return m_user_ids.size();
}

std::size_t SocialNetwork::post_count() const
{
    return m_post_ids.size();
}
