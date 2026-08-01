#include <iostream>
#include <string>

#include <estructuras/AVLTree.hpp>

struct UserData {
    std::string name;

    UserData(const std::string& _name = "")
        : name(_name)
    {
    }
};

int main()
{
    std::cout << "=== Prueba de AVLTree<int, UserData> ===" << std::endl;
    AVLTree<int, UserData> tree;

    tree.insert(30, UserData("Ana"));
    tree.insert(10, UserData("Luis"));
    tree.insert(20, UserData("Pedro"));
    tree.insert(40, UserData("Maria"));
    tree.insert(50, UserData("Jose"));

    std::cout << "Tamaño: " << tree.size() << std::endl;
    std::cout << "Altura: " << tree.height() << std::endl;

    UserData* data = tree.search(20);
    if (data != nullptr)
        std::cout << "Busqueda 20: " << data->name << std::endl;

    tree.remove(30);
    std::cout << "Después de borrar 30, tamaño: " << tree.size() << std::endl;
    std::cout << "Altura: " << tree.height() << std::endl;

    const UserData* const_data = tree.search(40);
    if (const_data != nullptr)
        std::cout << "Busqueda 40: " << const_data->name << std::endl;

    return 0;
}