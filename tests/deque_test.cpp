#include <iostream>
#include <string>

#include <estructuras/Deque.hpp>

struct Person {
    std::string name;
    int age;

    Person(const std::string& _name = "", int _age = 0)
        : name(_name), age(_age)
    {
    }
};

int main()
{
    std::cout << "=== Prueba de Deque<int> ===" << std::endl;
    Deque<int> deque;

    deque.add_back(10);
    deque.add_back(20);
    deque.add_front(5);
    deque.add_back(30);

    std::cout << "Tamaño: " << deque.size() << std::endl;
    std::cout << "Frente: " << deque.front() << std::endl;
    std::cout << "Final: " << deque.back() << std::endl;

    deque.remove_front();
    deque.remove_back();

    std::cout << "Después de quitar extremos, frente: " << deque.front() << std::endl;
    std::cout << "Después de quitar extremos, final: " << deque.back() << std::endl;

    std::cout << "\n=== Prueba de Deque<Person> ===" << std::endl;
    Deque<Person> people;
    people.add_back(Person("Ana", 20));
    people.add_front(Person("Luis", 25));
    people.add_back(Person("Pedro", 30));

    std::cout << "Frente: " << people.front().name << std::endl;
    std::cout << "Final: " << people.back().name << std::endl;

    return 0;
}