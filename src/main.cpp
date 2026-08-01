#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include <generador/DatasetGenerator.hpp>
#include <servicios/Benchmark.hpp>
#include <servicios/SocialNetwork.hpp>

namespace {

int read_int(const std::string& _prompt)
{
    std::cout << _prompt;
    int value = 0;

    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida, intenta de nuevo: ";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string read_line(const std::string& _prompt)
{
    std::cout << _prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void print_user_row(const User& _user)
{
    std::cout << std::left << std::setw(6) << _user.id()
               << std::setw(24) << _user.name()
               << std::setw(28) << _user.email()
               << std::setw(9) << _user.friend_count()
               << std::setw(9) << _user.post_count()
               << std::endl;
}

void print_user_table_header()
{
    std::cout << std::left << std::setw(6) << "ID"
               << std::setw(24) << "Nombre"
               << std::setw(28) << "Correo"
               << std::setw(9) << "Amigos"
               << std::setw(9) << "Posts"
               << std::endl;
}

void show_user_profile(SocialNetwork& _network, int _user_id)
{
    const User* user = _network.find_user(_user_id);
    if (user == nullptr) {
        std::cout << "No existe un usuario con ID " << _user_id << std::endl;
        return;
    }

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << " Perfil de " << user->name() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " ID:              " << user->id() << std::endl;
    std::cout << " Correo:          " << user->email() << std::endl;
    std::cout << " Registrado:      " << user->registration_date() << std::endl;
    std::cout << " Amigos:          " << user->friend_count() << std::endl;
    std::cout << " Publicaciones:   " << user->post_count() << std::endl;
    std::cout << " Reacciones recibidas: " << user->reactions_received() << std::endl;
    std::cout << "----------------------------------------\n" << std::endl;
}

void show_post_feed(SocialNetwork& _network, int _post_id)
{
    const Post* post = _network.find_post(_post_id);
    if (post == nullptr) {
        std::cout << "No existe una publicacion con ID " << _post_id << std::endl;
        return;
    }

    const User* owner = _network.find_user(post->owner_id());
    std::string owner_name = (owner != nullptr) ? owner->name() : "(usuario eliminado)";

    std::cout << "\n=========================================" << std::endl;
    std::cout << owner_name << " - " << post->date() << std::endl;
    std::cout << post->text() << std::endl;
    std::cout << "Likes: " << post->like_count() << "  Comentarios: " << post->comment_count() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    Vector<Comment> comments = _network.comments_of_post(_post_id);
    for (std::size_t i = 0; i < comments.size(); ++i) {
        const Comment& comment = comments[i];
        const User* author = _network.find_user(comment.author_id());
        std::string author_name = (author != nullptr) ? author->name() : "(usuario eliminado)";
        std::cout << "  > " << author_name << ": " << comment.text() << std::endl;
    }

    std::cout << "=========================================\n" << std::endl;
}

void users_menu(SocialNetwork& _network)
{
    int option = -1;

    while (option != 0) {
        std::cout << "\n--- Usuarios ---" << std::endl;
        std::cout << "1. Registrar usuario" << std::endl;
        std::cout << "2. Eliminar usuario" << std::endl;
        std::cout << "3. Buscar usuario por ID" << std::endl;
        std::cout << "4. Buscar usuario por nombre" << std::endl;
        std::cout << "5. Ver perfil de usuario" << std::endl;
        std::cout << "0. Volver" << std::endl;
        option = read_int("Opcion: ");

        switch (option) {
        case 1: {
            std::string name = read_line("Nombre: ");
            std::string email = read_line("Correo: ");
            std::string date = read_line("Fecha de registro (YYYY-MM-DD): ");
            int id = _network.register_user(name, email, date);
            std::cout << "Usuario registrado con ID " << id << std::endl;
            break;
        }
        case 2: {
            int id = read_int("ID del usuario a eliminar: ");
            std::cout << (_network.remove_user(id) ? "Usuario eliminado." : "No se encontro el usuario.") << std::endl;
            break;
        }
        case 3: {
            int id = read_int("ID a buscar: ");
            const User* user = _network.find_user(id);
            if (user == nullptr) {
                std::cout << "No se encontro el usuario." << std::endl;
            } else {
                print_user_table_header();
                print_user_row(*user);
            }
            break;
        }
        case 4: {
            std::string name = read_line("Nombre a buscar: ");
            Vector<int> ids = _network.search_users_by_name(name);
            if (ids.size() == 0) {
                std::cout << "No se encontraron usuarios con ese nombre." << std::endl;
            } else {
                print_user_table_header();
                for (std::size_t i = 0; i < ids.size(); ++i) {
                    const User* user = _network.find_user(ids[i]);
                    if (user != nullptr)
                        print_user_row(*user);
                }
            }
            break;
        }
        case 5: {
            int id = read_int("ID del usuario: ");
            show_user_profile(_network, id);
            break;
        }
        default:
            break;
        }
    }
}

void posts_menu(SocialNetwork& _network)
{
    int option = -1;

    while (option != 0) {
        std::cout << "\n--- Publicaciones ---" << std::endl;
        std::cout << "1. Crear publicacion" << std::endl;
        std::cout << "2. Eliminar publicacion" << std::endl;
        std::cout << "3. Ver publicaciones de un usuario" << std::endl;
        std::cout << "4. Reaccionar a una publicacion" << std::endl;
        std::cout << "5. Comentar una publicacion" << std::endl;
        std::cout << "6. Ver publicacion (feed)" << std::endl;
        std::cout << "0. Volver" << std::endl;
        option = read_int("Opcion: ");

        switch (option) {
        case 1: {
            int owner = read_int("ID del autor: ");
            std::string text = read_line("Texto: ");
            std::string date = read_line("Fecha (YYYY-MM-DD): ");
            int id = _network.create_post(owner, text, date);
            std::cout << (id == -1 ? "No existe ese usuario." : "Publicacion creada con ID " + std::to_string(id)) << std::endl;
            break;
        }
        case 2: {
            int id = read_int("ID de la publicacion a eliminar: ");
            std::cout << (_network.remove_post(id) ? "Publicacion eliminada." : "No se encontro la publicacion.") << std::endl;
            break;
        }
        case 3: {
            int owner = read_int("ID del usuario: ");
            Vector<int> posts = _network.posts_of_user(owner);
            std::cout << "Publicaciones: ";
            for (std::size_t i = 0; i < posts.size(); ++i)
                std::cout << posts[i] << " ";
            std::cout << std::endl;
            break;
        }
        case 4: {
            int id = read_int("ID de la publicacion: ");
            std::cout << (_network.react_to_post(id) ? "Reaccion agregada." : "No se encontro la publicacion.") << std::endl;
            break;
        }
        case 5: {
            int post_id = read_int("ID de la publicacion: ");
            int author_id = read_int("ID del autor del comentario: ");
            std::string text = read_line("Comentario: ");
            std::string date = read_line("Fecha (YYYY-MM-DD): ");
            int id = _network.comment_on_post(post_id, author_id, text, date);
            std::cout << (id == -1 ? "No se pudo comentar (revisa los IDs)." : "Comentario agregado.") << std::endl;
            break;
        }
        case 6: {
            int id = read_int("ID de la publicacion: ");
            show_post_feed(_network, id);
            break;
        }
        default:
            break;
        }
    }
}

void friends_menu(SocialNetwork& _network)
{
    int option = -1;

    while (option != 0) {
        std::cout << "\n--- Amistades ---" << std::endl;
        std::cout << "1. Agregar amigo" << std::endl;
        std::cout << "2. Eliminar amigo" << std::endl;
        std::cout << "3. Camino de amistad entre dos usuarios" << std::endl;
        std::cout << "4. Amigos en comun" << std::endl;
        std::cout << "5. Sugerencias de amistad" << std::endl;
        std::cout << "0. Volver" << std::endl;
        option = read_int("Opcion: ");

        switch (option) {
        case 1: {
            int a = read_int("ID usuario A: ");
            int b = read_int("ID usuario B: ");
            std::cout << (_network.add_friend(a, b) ? "Amistad creada." : "No se pudo crear la amistad.") << std::endl;
            break;
        }
        case 2: {
            int a = read_int("ID usuario A: ");
            int b = read_int("ID usuario B: ");
            std::cout << (_network.remove_friend(a, b) ? "Amistad eliminada." : "No se pudo eliminar la amistad.") << std::endl;
            break;
        }
        case 3: {
            int a = read_int("ID usuario A: ");
            int b = read_int("ID usuario B: ");
            Vector<int> path = _network.friendship_path(a, b);
            if (path.size() == 0) {
                std::cout << "No existe un camino de amistad entre ambos." << std::endl;
            } else {
                std::cout << "Camino (" << path.size() - 1 << " saltos): ";
                for (std::size_t i = 0; i < path.size(); ++i)
                    std::cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
            }
            break;
        }
        case 4: {
            int a = read_int("ID usuario A: ");
            int b = read_int("ID usuario B: ");
            Vector<int> common = _network.common_friends(a, b);
            std::cout << "Amigos en comun (" << common.size() << "): ";
            for (std::size_t i = 0; i < common.size(); ++i)
                std::cout << common[i] << " ";
            std::cout << std::endl;
            break;
        }
        case 5: {
            int id = read_int("ID del usuario: ");
            Vector<int> suggestions = _network.friend_suggestions(id);
            std::cout << "Sugerencias: ";
            for (std::size_t i = 0; i < suggestions.size(); ++i)
                std::cout << suggestions[i] << " ";
            std::cout << std::endl;
            break;
        }
        default:
            break;
        }
    }
}

void stats_menu(SocialNetwork& _network)
{
    int option = -1;

    while (option != 0) {
        std::cout << "\n--- Estadisticas / Rendimiento ---" << std::endl;
        std::cout << "1. Usuarios mas activos" << std::endl;
        std::cout << "2. Publicaciones con mas reacciones" << std::endl;
        std::cout << "3. Ejecutar benchmark (exporta CSV)" << std::endl;
        std::cout << "0. Volver" << std::endl;
        option = read_int("Opcion: ");

        switch (option) {
        case 1: {
            int top_k = read_int("Cuantos mostrar: ");
            Vector<int> top = _network.most_active_users(static_cast<std::size_t>(top_k));
            print_user_table_header();
            for (std::size_t i = 0; i < top.size(); ++i) {
                const User* user = _network.find_user(top[i]);
                if (user != nullptr)
                    print_user_row(*user);
            }
            break;
        }
        case 2: {
            int top_k = read_int("Cuantos mostrar: ");
            Vector<int> top = _network.most_reacted_posts(static_cast<std::size_t>(top_k));
            for (std::size_t i = 0; i < top.size(); ++i) {
                const Post* post = _network.find_post(top[i]);
                if (post != nullptr)
                    std::cout << "Post " << post->id() << " (" << post->like_count() << " likes): "
                               << post->text() << std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Ejecutando benchmark, esto puede tardar unos segundos..." << std::endl;
            Benchmark::run("data/generador/benchmark_results.csv");
            break;
        }
        default:
            break;
        }
    }
}

void dataset_menu(SocialNetwork& _network)
{
    int option = -1;

    while (option != 0) {
        std::cout << "\n--- Dataset ---" << std::endl;
        std::cout << "1. Generar dataset sintetico" << std::endl;
        std::cout << "0. Volver" << std::endl;
        option = read_int("Opcion: ");

        if (option == 1) {
            int users = read_int("Cantidad de usuarios a generar: ");
            int friends = read_int("Amigos promedio por usuario: ");
            int posts = read_int("Publicaciones promedio por usuario: ");

            DatasetGenerator generator;
            generator.generate(_network, static_cast<std::size_t>(users), static_cast<std::size_t>(friends), static_cast<std::size_t>(posts));

            std::cout << "Dataset generado. Usuarios totales: " << _network.user_count()
                       << ", publicaciones totales: " << _network.post_count() << std::endl;
        }
    }
}

} // namespace

int main()
{
    SocialNetwork network;
    int option = -1;

    while (option != 0) {
        std::cout << "\n=========================================" << std::endl;
        std::cout << " Mini Red Social" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "1. Usuarios" << std::endl;
        std::cout << "2. Publicaciones" << std::endl;
        std::cout << "3. Amistades" << std::endl;
        std::cout << "4. Estadisticas / Rendimiento" << std::endl;
        std::cout << "5. Dataset" << std::endl;
        std::cout << "0. Salir" << std::endl;
        option = read_int("Opcion: ");

        switch (option) {
        case 1:
            users_menu(network);
            break;
        case 2:
            posts_menu(network);
            break;
        case 3:
            friends_menu(network);
            break;
        case 4:
            stats_menu(network);
            break;
        case 5:
            dataset_menu(network);
            break;
        default:
            break;
        }
    }

    std::cout << "Hasta luego." << std::endl;
    return 0;
}
