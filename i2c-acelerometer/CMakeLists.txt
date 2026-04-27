cmake_minimum_required(VERSION 3.15)

project(hello_world C)

# Estándar de C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


set ( INCLUDES_DIR  
    inc
    inc/i2c_manager
    inc/l3g4200d_manager
)

set ( SOURCES_CODE
    src/main.c
    src/i2c_manager/i2c_driver.c
    src/l3g4200d_manager/l3g4200d_driver.c
)

# Incluir headers
include_directories(${INCLUDES_DIR})

# Crear ejecutable
add_executable(i2c_acelerometer
    ${SOURCES_CODE}
)