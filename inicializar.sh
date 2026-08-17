#!/bin/bash

DIRETORIO="$(dirname "$(realpath "$0")")"

echo "Iniciando os sistemas do Arcade..."

sudo "$DIRETORIO/Teclado" &

PID_TECLADO=$!

"$DIRETORIO/Arcade"

echo "Arcade fechado. Encerrando o teclado virtual..."
sudo kill $PID_TECLADO