#!/bin/bash

set -e

PROJECT_DIR=$1

if [ -z "$PROJECT_DIR" ]; then
echo "Uso: $0 <nombre_proyecto>"
exit 1
fi

PROJECT_DIR=${PROJECT_DIR%/}

echo "Iniciando deploy del proyecto $PROJECT_DIR..."

if [ ! -d "$PROJECT_DIR" ]; then
echo "Proyecto no encontrado: $PROJECT_DIR"
exit 1
fi

CONFIG_FILE="./conf/ssh_connection.conf"

if [ ! -f "$CONFIG_FILE" ]; then
echo "No existe el archivo:"
echo "$CONFIG_FILE"
exit 1
fi

source "$CONFIG_FILE"

BUILD_DIR="$PROJECT_DIR/build"

if [ ! -d "$BUILD_DIR" ]; then
echo "No existe el directorio build:"
echo "$BUILD_DIR"
exit 1
fi

EXECUTABLE="$BUILD_DIR/$PROJECT_DIR"

if [ ! -f "$EXECUTABLE" ]; then
echo "No existe el ejecutable:"
echo "$EXECUTABLE"
exit 1
fi

BIN_NAME=$(basename "$EXECUTABLE")

echo ""
echo "==================================="
echo "Proyecto      : $PROJECT_DIR"
echo "Ejecutable    : $BIN_NAME"
echo "SSH_USER      : $SSH_USER"
echo "SSH_HOST      : $SSH_HOST"
echo "SSH_PORT      : $SSH_PORT"
echo "SSH_KEY_PATH  : $SSH_KEY_PATH"
echo "==================================="

echo ""
echo "Copiando ejecutable..."

scp -P "$SSH_PORT" \
    -i "$SSH_KEY_PATH" \
    "$EXECUTABLE" \
    "$SSH_USER@$SSH_HOST:/tmp/$BIN_NAME"

echo ""
echo "Instalando en /usr/bin..."

sshpass -p "$DEVICE_PASSWORD" ssh \
    -p "$SSH_PORT" \
    -o StrictHostKeyChecking=no \
    "$SSH_USER@$SSH_HOST" \
    "echo '$DEVICE_PASSWORD' | sudo -S mv /tmp/$BIN_NAME /usr/bin/$BIN_NAME && sudo chmod +x /usr/bin/$BIN_NAME"

echo ""
echo "Deploy completado."
echo ""
echo "Ejecutable instalado:"
echo "/usr/bin/$BIN_NAME"

exit 0
