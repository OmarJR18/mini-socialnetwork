#ifndef DATASETGENERATOR_HPP
#define DATASETGENERATOR_HPP

#include <cstddef>

#include <servicios/SocialNetwork.hpp>

// Genera un dataset sintetico (usuarios, amistades y publicaciones) para
// poder probar la red social a mayor escala sin depender de un dataset
// externo. La aleatoriedad usa un generador congruencial lineal propio
// (mismo tipo que se ve en el curso) en vez de <random>, para no salirnos
// de lo permitido por el enunciado.
class DatasetGenerator {
private:
    unsigned long m_seed;

    unsigned long next_random();
    int random_range(int _min, int _max);

public:
    explicit DatasetGenerator(unsigned long _seed = 12345UL);

    void generate(SocialNetwork& _network, std::size_t _user_count, std::size_t _avg_friends_per_user, std::size_t _avg_posts_per_user);
};

#endif // DATASETGENERATOR_HPP
