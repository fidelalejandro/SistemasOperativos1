cmake_minimum_required(VERSION 3.23)
project(Practica4 C)

set(CMAKE_C_STANDARD 23)

add_compile_options(-lm)
add_executable(Practica4 main.c )
target_link_libraries(Practica4 m)