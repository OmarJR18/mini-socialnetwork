#include <servicios/Benchmark.hpp>

#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>

#include <generador/DatasetGenerator.hpp>
#include <servicios/SocialNetwork.hpp>

namespace {

double elapsed_ms(const std::chrono::steady_clock::time_point& _start, const std::chrono::steady_clock::time_point& _end)
{
    return std::chrono::duration<double, std::milli>(_end - _start).count();
}

} // namespace

void Benchmark::run(const std::string& _csv_path)
{
    std::size_t sizes[] = { 1000, 5000, 10000 };
    const std::size_t sizes_count = 3;
    const int trials = 5;

    std::ofstream csv(_csv_path.c_str());
    if (!csv.is_open()) {
        std::cout << "No se pudo abrir " << _csv_path << " para escritura." << std::endl;
        return;
    }

    csv << "usuarios,generacion_ms,camino_amistad_ms,amigos_comunes_ms,sugerencias_ms,usuarios_activos_ms,posts_reaccionados_ms\n";

    for (std::size_t s = 0; s < sizes_count; ++s) {
        std::size_t user_count = sizes[s];

        SocialNetwork network;
        DatasetGenerator generator;

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        generator.generate(network, user_count, 8, 3);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double generation_ms = elapsed_ms(start, end);

        // camino de amistad: promedio sobre varios pares espaciados a lo largo
        // del rango de usuarios, para que el resultado no dependa de un solo
        // par fijo que puede ser atipico
        start = std::chrono::steady_clock::now();
        for (int t = 0; t < trials; ++t) {
            int a = static_cast<int>(user_count * (t + 1) / (trials + 1));
            int b = static_cast<int>(user_count * (t + 2) / (trials + 1));
            network.friendship_path(a, b);
        }
        end = std::chrono::steady_clock::now();
        double path_ms = elapsed_ms(start, end) / trials;

        // amigos en comun: mismo criterio, varios pares para promediar
        start = std::chrono::steady_clock::now();
        for (int t = 0; t < trials; ++t) {
            int a = static_cast<int>(user_count * (t + 1) / (trials + 1));
            int b = static_cast<int>(user_count * (t + 2) / (trials + 1));
            network.common_friends(a, b);
        }
        end = std::chrono::steady_clock::now();
        double common_ms = elapsed_ms(start, end) / trials;

        // sugerencias: varios usuarios en distintos puntos del rango
        start = std::chrono::steady_clock::now();
        for (int t = 0; t < trials; ++t) {
            int uid = static_cast<int>(user_count * (t + 1) / (trials + 1));
            network.friend_suggestions(uid);
        }
        end = std::chrono::steady_clock::now();
        double suggestions_ms = elapsed_ms(start, end) / trials;

        start = std::chrono::steady_clock::now();
        network.most_active_users(10);
        end = std::chrono::steady_clock::now();
        double active_ms = elapsed_ms(start, end);

        start = std::chrono::steady_clock::now();
        network.most_reacted_posts(10);
        end = std::chrono::steady_clock::now();
        double reacted_ms = elapsed_ms(start, end);

        std::cout << "Dataset de " << user_count << " usuarios: generacion=" << generation_ms
                   << "ms camino_amistad=" << path_ms << "ms" << std::endl;

        csv << user_count << "," << generation_ms << "," << path_ms << "," << common_ms << ","
            << suggestions_ms << "," << active_ms << "," << reacted_ms << "\n";
    }

    csv.close();
    std::cout << "Resultados exportados a " << _csv_path << std::endl;
}