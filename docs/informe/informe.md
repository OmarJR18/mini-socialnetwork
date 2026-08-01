# Informe Técnico — Mini Red Social

Curso: Algoritmos y Estructuras de Datos
Proyecto Final

## 1. Introducción

Este proyecto implementa el núcleo de funcionamiento de una red social
simplificada (usuarios, amistades, publicaciones, comentarios y
reacciones), replicando en software el tipo de operaciones que sostiene
una red social real: registrar personas, conectar amistades, publicar
contenido y calcular relaciones no triviales entre usuarios (camino de
amistad, amigos en común, sugerencias).

La restricción central del proyecto es que ninguna estructura de datos
proviene de la STL de C++: `Vector`, listas enlazadas, `Deque`, `AVLTree`,
`Heap`, `HashTable` y `Graph` están implementadas desde cero por el
equipo. Sobre esas siete estructuras se construye una capa de entidades
(`User`, `Post`, `Comment`) y un servicio orquestador (`SocialNetwork`)
que expone las funcionalidades pedidas en el enunciado a través de una
interfaz de consola (CLI).

## 2. Arquitectura del sistema

El sistema está organizado en cuatro capas:

```
+----------------------------------------------------------+
|                    CLI (src/main.cpp)                    |
|   menus jerarquicos: usuarios, publicaciones, amistades,  |
|   estadisticas/rendimiento, dataset                       |
+----------------------------------------------------------+
                          |
                          v
+----------------------------------------------------------+
|         Servicio orquestador: SocialNetwork                |
|   registrar/eliminar/buscar usuarios y publicaciones,      |
|   reacciones, comentarios, amistades, rankings             |
+----------------------------------------------------------+
                          |
          +---------------+----------------+
          v                                 v
+-------------------+           +--------------------------+
|  Entidades         |           |  Estructuras de datos     |
|  User, Post,        |           |  Vector, LinkedList,      |
|  Comment            |           |  Deque, AVLTree, Heap,    |
|  (include/modelo)   |           |  HashTable, Graph          |
+-------------------+           |  (include/estructuras)    |
                                  +--------------------------+
```

`SocialNetwork` mantiene:

- `HashTable<int, User>` y `HashTable<int, Post>` como índice primario
  por ID (búsqueda O(1) promedio).
- `HashTable<int, Comment>` para los comentarios, referenciados por ID
  desde cada `Post`.
- `AVLTree<std::string, LinkedList<int>>` como índice secundario por
  nombre (permite nombres repetidos: cada nodo del árbol guarda la lista
  de IDs de usuarios con ese nombre).
- `Graph<int>` para las relaciones de amistad, usado en camino de
  amistad (BFS), amigos en común y sugerencias.
- Dos `Vector<int>` (`m_user_ids`, `m_post_ids`) que llevan el registro
  de IDs activos, porque `HashTable` no expone iteración sobre sus
  entradas y se necesita recorrer "todos los usuarios" o "todas las
  publicaciones" para los rankings.

Los IDs de usuario, publicación y comentario son enteros autoincrementales
asignados por `SocialNetwork` y nunca se reutilizan, incluso después de
un borrado — así se evita que un ID viejo "resucite" en el índice por
nombre o en una lista de amigos que todavía no se actualizó.

`DatasetGenerator` (en `data/generador`) puebla una `SocialNetwork` con
datos sintéticos (usuarios, amistades y publicaciones) usando un
generador congruencial lineal propio, sin depender de `<random>`.
`Benchmark` (en `src/servicios`) mide con `<chrono>` el tiempo de las
operaciones más costosas sobre datasets de distinto tamaño y exporta los
resultados a un CSV.

## 3. Descripción de las estructuras utilizadas

| Estructura | Rol en el proyecto |
|---|---|
| `Vector<T>` | Arreglo dinámico no ordenado (capacidad duplicada al llenarse). Se usa para listas de amigos y publicaciones por usuario, resultados de búsquedas y colecciones de IDs. |
| `LinkedList<T>` | Lista simplemente enlazada. Se usa como cadena de colisiones dentro de `HashTable` y como buckets del índice por nombre en `AVLTree`. |
| `Deque<T>` | Cola de doble extremo con buffer circular. Se usa como cola FIFO en el BFS de `Graph::shortest_path`. |
| `AVLTree<K,V>` | Árbol binario de búsqueda balanceado. Se usa como índice secundario de usuarios por nombre (`std::string` -> lista de IDs). |
| `Heap<T>` | Max-heap genérico. Se usa para los rankings top-K (usuarios más activos, publicaciones con más reacciones, sugerencias de amistad ordenadas por amigos en común). |
| `HashTable<K,V>` | Tabla hash con manejo de colisiones por encadenamiento (usa `LinkedList` internamente) y rehash automático. Es el índice primario de usuarios y publicaciones por ID, y la representación de la lista de adyacencia del grafo. |
| `Graph<K>` | Grafo no dirigido (lista de adyacencia sobre `HashTable`). Modela la red de amistades: BFS para camino más corto, intersección de vecinos para amigos en común, y conteo de amigos en común para sugerencias. |

## 4. Complejidad computacional

Sea `n` el número de usuarios y `d` el grado promedio (cantidad de
amigos) de un usuario.

| Operación | Complejidad | Notas |
|---|---|---|
| `Vector::add_item` | O(1) amortizado | duplica capacidad al llenarse |
| `Vector::remove_index` | O(1) | swap con el último elemento |
| `Vector::search` | O(n) | lineal |
| `LinkedList::add_last` / `add_first` | O(1) | mantiene puntero a la cola |
| `LinkedList::search` / `remove_item` | O(n) | lineal |
| `Deque::add_front` / `add_back` | O(1) amortizado | buffer circular con resize |
| `AVLTree::insert` / `search` / `remove` | O(log n) | balanceado por rotaciones |
| `Heap::insert` / `extract_top` | O(log k) | k = elementos en el heap |
| `HashTable::put` / `search` / `remove` | O(1) promedio, O(n) peor caso | depende de la calidad del hash y el factor de carga |
| `SocialNetwork::register_user` | O(1) promedio | inserción en `HashTable` + `AVLTree` (O(log u), u = nombres distintos) |
| `SocialNetwork::add_friend` / `remove_friend` | O(d) | recorre la lista de adyacencia para evitar aristas duplicadas |
| `SocialNetwork::friendship_path` (BFS) | O(n + a) | a = número de aristas del grafo |
| `SocialNetwork::common_friends` | O(d) | intersección lineal de dos listas de adyacencia |
| `SocialNetwork::friend_suggestions` | O(n · d) | recorre todos los vértices y, para cada uno, su lista de adyacencia |
| `SocialNetwork::most_active_users` / `most_reacted_posts` | O(n log n) | inserta todos los elementos en un `Heap` y extrae el top-k |

El punto más débil identificado es `friend_suggestions`: la implementación
actual del `Graph` recorre **todos** los vértices del grafo por cada
consulta (no solo los amigos de los amigos), lo que la vuelve O(n · d) en
vez de O(d²). Esto se confirma en los resultados experimentales de la
sección 5 y queda documentado aquí como una limitación conocida, no como
un error oculto.

## 5. Resultados experimentales

Los tiempos se midieron con `Benchmark::run`, que genera un dataset
sintético con `DatasetGenerator` (8 amigos promedio y 3 publicaciones
promedio por usuario) y mide con `<chrono>` seis operaciones sobre tres
tamaños de dataset. Ejecutado en la máquina de desarrollo del equipo:

| Usuarios | Generación (ms) | Camino de amistad (ms) | Amigos en común (ms) | Sugerencias (ms) | Usuarios más activos (ms) | Posts más reaccionados (ms) |
|---|---|---|---|---|---|---|
| 1 000 | 27.76 | 0.38 | 0.11 | 26.13 | 0.13 | 0.40 |
| 5 000 | 500.07 | 2.06 | 0.24 | 304.95 | 0.17 | 0.71 |
| 10 000 | 1 898.13 | 3.59 | 0.31 | 703.10 | 0.34 | 1.25 |

Observaciones:

- El **camino de amistad (BFS)** y los **rankings top-K** escalan bien:
  crecen mucho más lento que el tamaño del dataset, consistente con sus
  complejidades O(n + a) y O(n log n).
- Las **sugerencias de amistad** son, con claridad, la operación más
  costosa y la que peor escala (crece casi proporcional al número de
  usuarios), confirmando el análisis de la sección 4.
- La **generación del dataset** domina el tiempo total porque cada
  `add_friend`/`create_post` pasa por varias estructuras (HashTable, AVL,
  Graph); es esperable y no es una operación que el usuario final repita
  en un flujo normal de uso.

Estos números se generan también en `data/generador/benchmark_results.csv`
al ejecutar la opción "Ejecutar benchmark" del menú de Estadísticas del
CLI.

## 6. Capturas del funcionamiento

Salida real del CLI (`mini_socialnetwork`) generando un dataset pequeño y
consultando el ranking de usuarios más activos:

```
--- Dataset ---
1. Generar dataset sintetico
0. Volver
Opcion: 1
Cantidad de usuarios a generar: 50
Amigos promedio por usuario: 5
Publicaciones promedio por usuario: 2
Dataset generado. Usuarios totales: 50, publicaciones totales: 100

--- Estadisticas / Rendimiento ---
1. Usuarios mas activos
2. Publicaciones con mas reacciones
3. Ejecutar benchmark (exporta CSV)
0. Volver
Opcion: 1
Cuantos mostrar: 5
ID    Nombre                  Correo                      Amigos   Posts
1     Pedro Flores            user1@mail.com              15       2
50    Ana Rios                user50@mail.com             5        2
49    Jose Vega               user49@mail.com             5        2
48    Sofia Gomez             user48@mail.com             5        2
47    Diego Torres            user47@mail.com             5        2
```

Perfil de usuario formateado:

```
----------------------------------------
 Perfil de Pedro Flores
----------------------------------------
 ID:              1
 Correo:          user1@mail.com
 Registrado:      2026-06-25
 Amigos:          15
 Seguidores:      0
 Publicaciones:   2
 Reacciones recibidas: 3
----------------------------------------
```

## 7. Conclusiones

- Fue posible construir toda la capa de datos de una red social (usuarios,
  amistades, publicaciones, comentarios, reacciones) usando únicamente
  estructuras propias, sin recurrir a la STL.
- Separar el proyecto en estructuras genéricas (`include/estructuras`),
  entidades de dominio (`include/modelo`) y un servicio orquestador
  (`include/servicios`) permitió que cada estructura se probara de forma
  aislada antes de integrarla, y que el servicio no dependiera de los
  detalles internos de cada estructura.
- El cuello de botella real del sistema no está en las estructuras base
  (todas se comportan según su complejidad teórica) sino en un algoritmo
  de más alto nivel (`friend_suggestions`), lo cual es el tipo de hallazgo
  que solo aparece al medir con datasets de tamaño creciente en vez de
  asumir la complejidad sobre el papel.
- Quedan como trabajo futuro fuera del alcance de este proyecto: optimizar
  `friend_suggestions` para que recorra solo amigos-de-amigos en vez de
  todos los vértices, y probar el sistema con datasets de cientos de miles
  de usuarios (el diseño está pensado para escalar a ese tamaño, pero se
  validó experimentalmente hasta 10 000 usuarios por el tiempo disponible
  para el proyecto).
