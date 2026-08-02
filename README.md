# Mini Red Social

Proyecto final del curso **Algoritmos y Estructuras de Datos**.

## Descripcion

Implementacion desde cero (sin STL) del nucleo de una red social: usuarios,
amistades, publicaciones, comentarios y reacciones. Toda estructura de datos
utilizada fue disenada e implementada por el equipo, sin recurrir a
`std::vector`, `std::list`, `std::map`, `std::unordered_map`, `std::set`,
`std::queue`, `std::stack`, `std::sort`, `std::find` ni contenedores o
algoritmos equivalentes de la STL.

Las unicas librerias externas permitidas son `<chrono>` para medicion de
tiempos y librerias auxiliares para lectura de CSV/JSON si hicieran falta.

## Estructura del repositorio

```
include/estructuras/    # Headers de las estructuras de datos (header-only)
include/modelo/         # User, Post, Comment
include/servicios/      # SocialNetwork (orquestador) y Benchmark
src/modelo/             # Implementacion de User, Post, Comment
src/servicios/          # Implementacion de SocialNetwork y Benchmark
src/main.cpp            # CLI interactivo
data/generador/         # Generador sintetico de dataset
tests/                  # Pruebas unitarias por modulo
docs/                   # Enunciado del proyecto e informe tecnico
```

## Estructuras implementadas

| Estructura | Archivo | Uso en el sistema |
|---|---|---|
| `Vector<T>` | `Vector.hpp` | Arreglo dinamico con redimension O(1) amortizado. Base para listas de amigos, publicaciones, IDs |
| `LinkedList<T>` | `LinkedList.hpp` | Lista simple enlazada. Encadenamiento en HashTable, listas auxiliares |
| `DoublyLinkedList<T>` | `DoublyLinkedList.hpp` | Lista doblemente enlazada |
| `CircularLinkedList<T>` | `CircularLinkedList.hpp` | Lista circular |
| `Deque<T>` | `Deque.hpp` | Cola doble (buffer circular). BFS en Graph |
| `AVLTree<K,V>` | `AVLTree.hpp` | Arbol AVL balanceado. Busqueda de usuarios por nombre (indice secundario) |
| `Heap<T>` | `Heap.hpp` | Max-heap generico. Rankings top-K y cola de prioridad para sugerencias |
| `HashTable<K,V>` | `HashTable.hpp` | Tabla hash con functor intercambiable, encadenamiento con `LinkedList`. Acceso O(1) a usuarios, posts y comentarios |
| `Graph<K>` | `Graph.hpp` | Lista de adyacencia (via `HashTable`). BFS para camino mas corto, amigos en comun, sugerencias |

## Compilacion

```bash
mkdir build && cd build
cmake ..
make
./mini_socialnetwork
```

## Funcionalidades

### CLI jerarquico

- **Usuarios**: registrar, eliminar, buscar por ID, buscar por nombre, perfil formateado
- **Publicaciones**: crear, eliminar, ver feed con comentarios, reaccionar, comentar
- **Amistades**: agregar/eliminar amigos, camino de amistad (BFS), amigos en comun, sugerencias
- **Estadisticas / Rendimiento**: top-K usuarios mas activos, top-K posts mas reaccionados, ejecutar benchmark (exporta a CSV)
- **Dataset**: generar dataset sintetico configurable (usuarios, promedio de amigos y publicaciones por usuario)

### Dataset sintetico

Generador propio en `data/generador/`. Datos sinteticos: IDs secuenciales, nombres
generados a partir de listas fijas, emails unicos, fechas en formato
`YYYY-MM-DD`. El grafo de amistades se construye incrementalmente: cada nuevo
usuario se conecta con usuarios ya existentes mediante un generador congruencial
lineal (mismo tipo visto en el curso). Parametros configurables desde la CLI.

### Benchmark

Mide el tiempo de las operaciones mas costosas (camino de amistad, amigos en comun,
sugerencias, rankings) sobre datasets de 1K, 5K y 10K usuarios. Ejecuta 5 repeticiones por
operacion, con pares espaciados a lo largo del rango de usuarios, y exporta a CSV.
Resultados exportados a CSV para los graficos del informe estadistico.

## Restricciones

- Cero STL para estructuras de datos o algoritmos
- Sin manejo de excepciones (`try/catch`)
- Codigo en ingles (clases, metodos, variables), comentarios y CLI en espanol
- Nombres segun la convencion del curso: `m_atributo`, `_parametro`, `snake_case`
- Librerias externas solo para IO, medicion de tiempos y graficos

## Pruebas

Cada estructura y modulo tiene su prueba unitaria en `tests/`:

```bash
make vector_test linkedlist_test doublylinkedlist_test circularlinkedlist_test deque_test avl_test heap_test hashtable_test graph_test modelo_test socialnetwork_test generador_test
for t in *_test; do ./$t; done
```