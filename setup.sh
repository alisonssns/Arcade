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
chmod +x inicializar.sh
chmod -R +rwx ./jogos

echo "=== 4. Configurando permissão sudo sem senha ==="
SCRIPT_PATH="$(realpath ./inicializar.sh)"
echo "$USER ALL=(ALL) NOPASSWD: $SCRIPT_PATH" | sudo tee /etc/sudoers.d/arcade_permissions > /dev/null
sudo chmod 0440 /etc/sudoers.d/arcade_permissions

echo "=== Instalação e configuração concluídas com sucesso! ==="