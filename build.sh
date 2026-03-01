#!/bin/bash

set -e
projectyDir=$1
# Eliminar la barra final si existe
projectyDir=${projectyDir%/} 

#verificar si se ha proporcionado un argumento
if [ -z "$projectyDir" ]; then
    echo "Uso: $0 <directorio_del_proyecto>"
    exit 1
fi

#verificar si el directorio del proyecto existe
if [ ! -d "$projectyDir" ]; then
    echo "El directorio del proyecto no existe."
    exit 1
fi

# Verificar si el directorio de build existe
if [ -e "$projectyDir/build" ]; then
    echo "El directorio de build ya existe. Eliminando..."
    rm -rf "$projectyDir/build"
    mkdir -p "$projectyDir/build"
else
    echo "El directorio de build no existe. Creando..."
    mkdir -p "$projectyDir/build"
fi

# Cambiar al directorio de build
cd "$projectyDir/build"

# Ejecutar CMake para configurar el proyecto
cmake ..
cmake --build . --config Release

exit 0