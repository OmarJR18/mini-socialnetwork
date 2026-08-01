#include <iostream>

#include <generador/DatasetGenerator.hpp>
#include <servicios/SocialNetwork.hpp>

int main()
{
    std::cout << "=== Prueba de DatasetGenerator ===" << std::endl;

    SocialNetwork network;
    DatasetGenerator generator;

    generator.generate(network, 200, 6, 2);

    std::cout << "Usuarios generados: " << network.user_count() << std::endl;
    std::cout << "Publicaciones generadas: " << network.post_count() << std::endl;

    Vector<int> active = network.most_active_users(5);
    std::cout << "Top 5 usuarios mas activos: ";
    for (std::size_t i = 0; i < active.size(); ++i)
        std::cout << active[i] << " ";
    std::cout << std::endl;

    Vector<int> reacted = network.most_reacted_posts(5);
    std::cout << "Top 5 publicaciones con mas reacciones: ";
    for (std::size_t i = 0; i < reacted.size(); ++i)
        std::cout << reacted[i] << " ";
    std::cout << std::endl;

    Vector<int> path = network.friendship_path(1, 200);
    std::cout << "Camino de amistad 1->200 (longitud " << path.size() << "): ";
    for (std::size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << std::endl;

    return 0;
}
