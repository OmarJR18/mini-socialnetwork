#include <generador/DatasetGenerator.hpp>

#include <cstdio>
#include <string>

DatasetGenerator::DatasetGenerator(unsigned long _seed)
    : m_seed(_seed)
{
}

unsigned long DatasetGenerator::next_random()
{
    // generador congruencial lineal (parametros clasicos de glibc)
    m_seed = (m_seed * 1103515245UL + 12345UL) & 0x7FFFFFFFUL;
    return m_seed;
}

int DatasetGenerator::random_range(int _min, int _max)
{
    if (_max <= _min)
        return _min;

    unsigned long span = static_cast<unsigned long>(_max - _min + 1);
    return _min + static_cast<int>(next_random() % span);
}

void DatasetGenerator::generate(SocialNetwork& _network, std::size_t _user_count, std::size_t _avg_friends_per_user, std::size_t _avg_posts_per_user)
{
    _network = SocialNetwork();

    static const char* first_names[] = {
        "Ana", "Luis", "Maria", "Jose", "Carla", "Pedro", "Lucia", "Diego", "Sofia", "Mateo"
    };
    static const char* last_names[] = {
        "Torres", "Rios", "Paz", "Gomez", "Diaz", "Rojas", "Flores", "Castro", "Vega", "Leon"
    };
    const int first_count = 10;
    const int last_count = 10;

    Vector<int> ids(_user_count > 0 ? _user_count : 1);

    for (std::size_t i = 0; i < _user_count; ++i) {
        std::string name = std::string(first_names[random_range(0, first_count - 1)])
            + " " + last_names[random_range(0, last_count - 1)];

        char email[64];
        std::snprintf(email, sizeof(email), "user%zu@mail.com", i + 1);

        char date[16];
        std::snprintf(date, sizeof(date), "2026-%02d-%02d", random_range(1, 12), random_range(1, 28));

        int id = _network.register_user(name, email, date);
        ids.add_item(id);

        // cada usuario nuevo se conecta con usuarios que ya existian, asi
        // el grafo crece sin depender de aristas hacia el futuro
        for (std::size_t f = 0; f < _avg_friends_per_user && ids.size() > 1; ++f) {
            int other = ids[static_cast<std::size_t>(random_range(0, static_cast<int>(ids.size()) - 2))];
            _network.add_friend(id, other);
        }
    }

    for (std::size_t i = 0; i < ids.size(); ++i) {
        int owner = ids[i];

        for (std::size_t p = 0; p < _avg_posts_per_user; ++p) {
            char date[16];
            std::snprintf(date, sizeof(date), "2026-%02d-%02d", random_range(1, 12), random_range(1, 28));

            std::string text = "Publicacion #" + std::to_string(p + 1) + " del usuario " + std::to_string(owner);
            int post_id = _network.create_post(owner, text, date);
            if (post_id == -1)
                continue;

            int likes = random_range(0, 10);
            for (int l = 0; l < likes; ++l)
                _network.react_to_post(post_id);

            int comments = random_range(0, 2);
            for (int c = 0; c < comments; ++c) {
                int author = ids[static_cast<std::size_t>(random_range(0, static_cast<int>(ids.size()) - 1))];
                _network.comment_on_post(post_id, author, "Comentario " + std::to_string(c + 1), date);
            }
        }
    }
}
