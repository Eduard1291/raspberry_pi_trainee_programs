#!/bin/bash

set -e

PROJECT_DIR=$1

# Comprueba y remueve la barra final si esta agregada
PROJECT_DIR=${PROJECT_DIR%/}

# comprueba si los parametros de entrada son validos
if [ -z "$PROJECT_DIR" ]; then
    echo "Uso: $0 <directorio_proyecto>"
    exit 1
fi

# Comprueba si el directorio de construcción existe
if [ ! -d "$PROJECT_DIR" ]; then
    echo "No existe el proyecto: $PROJECT_DIR"
    exit 1
fi

BUILD_DIR="$PROJECT_DIR/build"

echo "Limpiando build..."
rm -rf "$BUILD_DIR"

mkdir -p "$BUILD_DIR"

TOOLCHAIN_FILE="$(pwd)/toolchain/raspberrypi64.cmake"

echo "Toolchain: $TOOLCHAIN_FILE"

cmake \
    -S "$PROJECT_DIR" \
    -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=Release

cmake --build "$BUILD_DIR" -j$(nproc)

echo ""
echo "Build completado."
exit 0