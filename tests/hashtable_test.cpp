#include <iostream>
#include <string>

#include <estructuras/HashTable.hpp>

struct Person {
    std::string name;
    int age;

    Person(const std::string& _name = "", int _age = 0)
        : name(_name), age(_age)
    {
    }
};

struct PersonHash {
    std::size_t operator()(const int& _key) const
    {
        return static_cast<std::size_t>(_key);
    }
};

int main()
{
    std::cout << "=== Prueba de HashTable<int, std::string> ===" << std::endl;
    HashTable<int, std::string> table(2);

    table.put(1, "uno");
    table.put(2, "dos");
    table.put(3, "tres");
    table.put(4, "cuatro");

    std::cout << "Tamaño: " << table.size() << std::endl;
    std::cout << "Capacidad: " << table.capacity() << std::endl;

    if (std::string* value = table.search(3))
        std::cout << "Clave 3: " << *value << std::endl;

    table.put(3, "tres actualizado");
    std::cout << "Clave 3 actualizada: " << *table.search(3) << std::endl;

    table.remove(2);
    std::cout << "Contiene 2: " << (table.contains(2) ? "si" : "no") << std::endl;

    std::cout << "\n=== Prueba de HashTable<std::string, Person> ===" << std::endl;
    HashTable<std::string, Person> people;
    people.put("ana", Person("Ana", 20));
    people.put("luis", Person("Luis", 25));

    if (Person* person = people.search("ana"))
        std::cout << person->name << ", " << person->age << std::endl;

    return 0;
}