#!/bin/bash

# Para o script se algum comando falhar
set -e

echo "=== 1. Baixando e extraindo os jogos ==="
wget -O jogos_arcade.zip "https://github.com/alisonssns/Arcade/releases/download/jogos/jogos.zip"
unzip -o jogos_arcade.zip
rm -f jogos_arcade.zip

echo "=== 2. Compilando os programas C++ ==="
g++ controle.cpp -o Teclado -std=c++20 -pthread
g++ main.cpp utils.cpp -o Arcade -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system

echo "=== 3. Ajustando permissões dos arquivos ==="
chmod +x Teclado
chmod +x Arcade
chmod -R +rwx ./jogos

echo "=== Instalação e compilação concluídas com sucesso! ==="