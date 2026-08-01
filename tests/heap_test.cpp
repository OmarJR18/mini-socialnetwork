#include <iostream>
#include <string>

#include <estructuras/Heap.hpp>

struct Player {
    std::string name;
    int score;

    Player(const std::string& _name = "", int _score = 0)
        : name(_name), score(_score)
    {
    }
};

struct PlayerScoreCompare {
    bool operator()(const Player& _a, const Player& _b) const
    {
        return _a.score < _b.score;
    }
};

int main()
{
    std::cout << "=== Prueba de Heap<int> ===" << std::endl;
    Heap<int> heap(2);

    heap.insert(15);
    heap.insert(3);
    heap.insert(40);
    heap.insert(8);
    heap.insert(25);

    std::cout << "Tamaño: " << heap.size() << std::endl;
    std::cout << "Top: " << heap.top() << std::endl;

    int value = 0;
    while (heap.extract_top(value))
        std::cout << value << " ";
    std::cout << std::endl;

    std::cout << "\n=== Prueba de Heap<Player> ===" << std::endl;
    Heap<Player, PlayerScoreCompare> players;
    players.insert(Player("Ana", 120));
    players.insert(Player("Luis", 300));
    players.insert(Player("Pedro", 210));

    std::cout << "Top jugador: " << players.top().name << " - " << players.top().score << std::endl;

    Player best;
    if (players.extract_top(best))
        std::cout << "Extraido: " << best.name << std::endl;

    return 0;
}