#include <iostream>

#include <servicios/SocialNetwork.hpp>

int main()
{
    std::cout << "=== Prueba de SocialNetwork ===" << std::endl;

    SocialNetwork network;

    int ana = network.register_user("Ana Torres", "ana@mail.com", "2026-07-01");
    int luis = network.register_user("Luis Paz", "luis@mail.com", "2026-07-02");
    int maria = network.register_user("Maria Rios", "maria@mail.com", "2026-07-03");

    network.add_friend(ana, luis);
    network.add_friend(luis, maria);

    std::cout << "Usuarios registrados: " << network.user_count() << std::endl;

    Vector<int> path = network.friendship_path(ana, maria);
    std::cout << "Camino de amistad Ana->Maria: ";
    for (std::size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << std::endl;

    Vector<int> suggestions = network.friend_suggestions(ana);
    std::cout << "Sugerencias para Ana: ";
    for (std::size_t i = 0; i < suggestions.size(); ++i)
        std::cout << suggestions[i] << " ";
    std::cout << std::endl;

    int post_id = network.create_post(ana, "Hola a todos!", "2026-07-04");
    network.react_to_post(post_id);
    network.react_to_post(post_id);
    network.comment_on_post(post_id, luis, "Que buena publicacion", "2026-07-04");

    const Post* post = network.find_post(post_id);
    std::cout << "Post de Ana: \"" << post->text() << "\" likes=" << post->like_count()
               << " comentarios=" << post->comment_count() << std::endl;

    Vector<int> active = network.most_active_users(2);
    std::cout << "Usuarios mas activos: ";
    for (std::size_t i = 0; i < active.size(); ++i)
        std::cout << active[i] << " ";
    std::cout << std::endl;

    Vector<int> found = network.search_users_by_name("Luis Paz");
    std::cout << "Busqueda por nombre 'Luis Paz': ";
    for (std::size_t i = 0; i < found.size(); ++i)
        std::cout << found[i] << " ";
    std::cout << std::endl;

    network.remove_user(luis);
    std::cout << "Usuarios tras eliminar a Luis: " << network.user_count() << std::endl;

    const User* ana_user = network.find_user(ana);
    std::cout << "Amigos de Ana tras eliminar a Luis: " << ana_user->friend_count() << std::endl;

    return 0;
}
