cmake_minimum_required(VERSION 3.15)

project(hello_world C)

# Estándar de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Incluir headers
include_directories(include)

# Crear ejecutable
add_executable(hello_world
    src/main.c
)