#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <string>

// Mide el tiempo de las operaciones mas costosas de SocialNetwork (BFS de
// camino de amistad, amigos en comun, sugerencias, rankings) sobre datasets
// de distinto tamano generados con DatasetGenerator, y exporta los
// resultados a un CSV para la seccion de resultados experimentales del
// informe.
class Benchmark {
public:
    static void run(const std::string& _csv_path);
};

#endif // BENCHMARK_HPP
