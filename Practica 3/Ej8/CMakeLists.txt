cmake_minimum_required(VERSION 3.00)
project(Ej8 C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -lm)
add_executable(Ej8 main.c)
target_link_libraries(Ej8 m)
