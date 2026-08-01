cmake_minimum_required(VERSION 3.10)
project(mini_socialnetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

# No libraries needed for now