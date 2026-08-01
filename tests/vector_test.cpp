#include <iostream>
#include <string>
#include <estructuras/Vector.hpp>

// Simple struct for testing with custom types
struct Person {
    std::string name;
    int age;

    // Constructor
    Person(const std::string& _name = "", int _age = 0)
        : name(_name), age(_age) {}

    // Equality operator for search
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

int main() {
    std::cout << "=== Prueba de Vector<int> ===" << std::endl;
    Vector<int> vecInt;
    std::cout << "Tamaño inicial: " << vecInt.size() << ", Capacidad: " << vecInt.capacity() << std::endl;

    // Agregar elementos
    for (int i = 0; i < 10; ++i) {
        vecInt.add_item(i * 10);
        std::cout << "Agregado " << i*10 << ", tamaño: " << vecInt.size()
                  << ", capacidad: " << vecInt.capacity() << std::endl;
    }

    // Acceso mediante operador[]
    std::cout << "Elementos en el vector: ";
    for (size_t i = 0; i < vecInt.size(); ++i) {
        std::cout << vecInt[i] << " ";
    }
    std::cout << std::endl;

    // Búsqueda
    int busqueda = 30;
    int indice = vecInt.search(busqueda);
    if (indice != -1) {
        std::cout << "Elemento " << busqueda << " encontrado en índice " << indice << std::endl;
    } else {
        std::cout << "Elemento " << busqueda << " no encontrado" << std::endl;
    }

    // Eliminar por índice
    std::cout << "Eliminando elemento en índice 2 (valor " << vecInt[2] << ")" << std::endl;
    vecInt.remove_index(2);
    std::cout << "Después de eliminar, tamaño: " << vecInt.size() << std::endl;
    std::cout << "Elementos ahora: ";
    for (size_t i = 0; i < vecInt.size(); ++i) {
        std::cout << vecInt[i] << " ";
    }
    std::cout << std::endl;

    // Prueba de copia
    std::cout << "\n--- Prueba de constructor de copia y asignación ---" << std::endl;
    Vector<int> vecCopiado(vecInt); // constructor de copia
    std::cout << "Vector copiado, tamaño: " << vecCopiado.size() << std::endl;
    Vector<int> vecAsignado;
    vecAsignado = vecInt; // operador de asignación
    std::cout << "Vector asignado, tamaño: " << vecAsignado.size() << std::endl;

    // Prueba con tipos personalizados
    std::cout << "\n=== Prueba de Vector<Person> ===" << std::endl;
    Vector<Person> vecPersona;
    vecPersona.add_item(Person("Ana", 20));
    vecPersona.add_item(Person("Luis", 25));
    vecPersona.add_item(Person("Pedro", 30));

    std::cout << "Personas en el vector:" << std::endl;
    for (size_t i = 0; i < vecPersona.size(); ++i) {
        std::cout << "  " << vecPersona[i].name << ", " << vecPersona[i].age << std::endl;
    }

    // Buscar una persona
    Person buscado("Luis", 25);
    int idx = vecPersona.search(buscado);
    if (idx != -1) {
        std::cout << "Persona encontrada en índice " << idx << std::endl;
    } else {
        std::cout << "Persona no encontrada" << std::endl;
    }

    // Eliminar una persona
    std::cout << "Eliminando persona en índice 0" << std::endl;
    vecPersona.remove_index(0);
    std::cout << "Después de eliminar, tamaño: " << vecPersona.size() << std::endl;

    return 0;
}