cmake_minimum_required(VERSION 3.10)
project(mini_socialnetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data)

# Executable for vector test
add_executable(vector_test tests/vector_test.cpp)

# Executable for linked list test
add_executable(linkedlist_test tests/linkedlist_test.cpp)

# Executable for deque test
add_executable(deque_test tests/deque_test.cpp)

# Executable for AVL test
add_executable(avl_test tests/avl_test.cpp)

# Executable for Heap test
add_executable(heap_test tests/heap_test.cpp)

# Executable for HashTable test
add_executable(hashtable_test tests/hashtable_test.cpp)

# Executable for Graph test
add_executable(graph_test tests/graph_test.cpp)

set(MODELO_SOURCES
    src/modelo/User.cpp
    src/modelo/Post.cpp
    src/modelo/Comment.cpp
)

set(SERVICIOS_SOURCES
    src/servicios/SocialNetwork.cpp
    src/servicios/Benchmark.cpp
)

set(GENERADOR_SOURCES
    data/generador/DatasetGenerator.cpp
)

# Executable for modelo (User/Post/Comment) test
add_executable(modelo_test tests/modelo_test.cpp ${MODELO_SOURCES})

# Executable for SocialNetwork test
add_executable(socialnetwork_test tests/socialnetwork_test.cpp ${MODELO_SOURCES} ${SERVICIOS_SOURCES} ${GENERADOR_SOURCES})

# Executable for the dataset generator test
add_executable(generador_test tests/generador_test.cpp ${MODELO_SOURCES} ${SERVICIOS_SOURCES} ${GENERADOR_SOURCES})

# Main CLI application
add_executable(mini_socialnetwork src/main.cpp ${MODELO_SOURCES} ${SERVICIOS_SOURCES} ${GENERADOR_SOURCES})

# No libraries needed for now