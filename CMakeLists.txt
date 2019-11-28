cmake_minimum_required(VERSION 3.10.2)
project(AVL_TREE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
add_executable(AVL_TREE AVL.cpp AVL.h avl_interface.cpp)