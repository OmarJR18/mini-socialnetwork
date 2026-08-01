#include <iostream>
#include <string>

#include <estructuras/DoublyLinkedList.hpp>

struct Person {
    std::string name;
    int age;

    Person(const std::string& _name = "", int _age = 0)
        : name(_name), age(_age)
    {
    }

    bool operator==(const Person& _other) const
    {
        return name == _other.name && age == _other.age;
    }
};

int main()
{
    std::cout << "=== Prueba de DoublyLinkedList<int> ===" << std::endl;
    DoublyLinkedList<int> list;

    list.add_last(10);
    list.add_last(20);
    list.add_first(5);
    list.add_last(30);

    std::cout << "Tamaño: " << list.size() << std::endl;
    std::cout << "Primero: " << list.at(0) << std::endl;
    std::cout << "Ultimo: " << list.at(list.size() - 1) << std::endl;

    list.remove_at(1);
    std::cout << "Tras borrar indice 1: ";
    for (std::size_t i = 0; i < list.size(); ++i)
        std::cout << list.at(i) << " ";
    std::cout << std::endl;

    std::cout << "Indice de 30: " << list.search(30) << std::endl;

    DoublyLinkedList<Person> people;
    people.add_last(Person("Ana", 20));
    people.add_first(Person("Luis", 25));

    std::cout << "\nPersonas: " << std::endl;
    for (std::size_t i = 0; i < people.size(); ++i)
        std::cout << people.at(i).name << ", " << people.at(i).age << std::endl;

    return 0;
}