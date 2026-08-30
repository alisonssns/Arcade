echo "=== Baixando e extraindo os jogos ==="
wget -O jogos_arcade.zip "https://github.com/alisonssns/Arcade/releases/download/jogos/jogos.zip"
unzip -o jogos_arcade.zip
rm -f jogos_arcade.zip
chmod -R +rwx ./jogos