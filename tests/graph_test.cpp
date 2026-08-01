#include <iostream>
#include <string>

#include <estructuras/Graph.hpp>

int main()
{
    std::cout << "=== Prueba de Graph<int> ===" << std::endl;
    Graph<int> graph;

    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.add_edge(3, 4);
    graph.add_edge(4, 5);

    std::cout << "Vertices: " << graph.vertex_count() << std::endl;
    std::cout << "Tiene arista 1-2: " << (graph.has_edge(1, 2) ? "si" : "no") << std::endl;

    Vector<int> path = graph.shortest_path(1, 5);
    std::cout << "Camino 1->5: ";
    for (std::size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << std::endl;

    Vector<int> common = graph.common_friends(2, 3);
    std::cout << "Amigos en comun 2 y 3: ";
    for (std::size_t i = 0; i < common.size(); ++i)
        std::cout << common[i] << " ";
    std::cout << std::endl;

    Vector<int> suggestions = graph.suggest_friends(1);
    std::cout << "Sugerencias para 1: ";
    for (std::size_t i = 0; i < suggestions.size(); ++i)
        std::cout << suggestions[i] << " ";
    std::cout << std::endl;

    graph.remove_edge(1, 2);
    std::cout << "Tiene arista 1-2 tras borrar: " << (graph.has_edge(1, 2) ? "si" : "no") << std::endl;

    return 0;
}