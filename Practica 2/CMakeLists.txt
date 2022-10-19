cmake_minimum_required(VERSION 3.23)
project(Practica2 C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -lm)

add_executable(Practica2 main.c)
target_link_libraries(Practica2 m)
