#include "config.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

const std::string SNES = "retroarch -L /home/alissonl/.config/retroarch/cores/snes9x_libretro.so";
const std::string GBA = "retroarch -L /home/alissonl/.config/retroarch/cores/mgba_libretro.so";
const std::string NEO = "retroarch -L /home/alissonl/.config/retroarch/cores/fbneo_libretro.so";
const std::string NES = "retroarch -L /home/alissonl/.config/retroarch/cores/nestopia_libretro.so";
const std::string flycast = "retroarch -L /home/alissonl/.config/retroarch/cores/flycast_libretro.so";
// Comunidade: jogos .exe rodam via Wine. O comando final fica "wine <caminho>.exe"
const std::string WINE = "wine";

std::string lower(std::string data)
{
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return data;
}

// Carrega as texturas/capas e calcula a escala de cada sprite.
// Usado tanto pelos emuladores quanto pela comunidade.
void prepararSprites(std::vector<Jogo> &lista)
{
    float larguraDesejada = 800.f;
    float alturaDesejada = 500.f;

    for (auto &jogo : lista)
    {
        std::string caminhoCapa = "/home/alissonl/Downloads/Arcade/public/assets/capas/" + jogo.imagem;

        if (!jogo.textura.loadFromFile(caminhoCapa))
        {
            std::cout << "Aviso: Capa nao encontrada para " << jogo.titulo << " em " << caminhoCapa << std::endl;

            jogo.sprite.emplace(jogo.textura);
            jogo.sprite->setTextureRect(sf::IntRect({0, 0}, {400, 300}));
            jogo.sprite->setColor(sf::Color(100, 100, 100));

            jogo.escalaBaseX = larguraDesejada / 400.f;
            jogo.escalaBaseY = alturaDesejada / 300.f;
        }
        else
        {
            jogo.sprite.emplace(jogo.textura);
            sf::Vector2u tamanhoTextura = jogo.textura.getSize();

            jogo.escalaBaseX = larguraDesejada / (float)tamanhoTextura.x;
            jogo.escalaBaseY = alturaDesejada / (float)tamanhoTextura.y;
        }

        sf::FloatRect bounds = jogo.sprite->getLocalBounds();
        jogo.sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }
}

std::vector<Jogo> carregarConfiguracoes()
{
    std::vector<Jogo> lista = {
        {.titulo = "Street Fighter 2", .emulador = SNES, .imagem = "sta2.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/sta2.sfc"},
        // {.titulo = "Metal Slug 3", .emulador = NEO, .imagem = "mslug3.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/mslug3.neo"},
        {.titulo = "Sonic 3", .emulador = GBA, .imagem = "sonic3.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/sonicadv3.gba"},
        // {.titulo = "The king of Fighters", .emulador = NEO, .imagem = "kof98.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/kof98.neo"},
        {.titulo = "Castlevania", .emulador = GBA, .imagem = "castlevania.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/castlevania.gba"},
        {.titulo = "Super Mario World", .emulador = SNES, .imagem = "smw.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/smw.sfc"},
        {.titulo = "Super Metroid", .emulador = SNES, .imagem = "metroid.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/metroidt.sfc"},
        {.titulo = "DOOM", .emulador = GBA, .imagem = "doom.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/doom.gba"},
        {.titulo = "Guilty Gear", .emulador = GBA, .imagem = "guilty.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/guilty.gba"},
        {.titulo = "Marvel vs Capcom 2", .emulador = flycast, .imagem = "mvsc2.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/mvsc2.chd"},
        {.titulo = "Pacman", .emulador = NES, .imagem = "pacman.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/pacman.nes"},
        {.titulo = "Super Bomberman 2", .emulador = SNES, .imagem = "Sbomberman2.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/Sbomberman2.sfc"},
        {.titulo = "Super Bomberman 3", .emulador = SNES, .imagem = "Sbomberman3.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/Sbomberman3.sfc"},

        // {.titulo = "GALAGA", .emulador = MAME, .imagem = "galaga.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/galagao.zip"},
        // {.titulo = "Asteroid", .emulador = MAME, .imagem = "asteroid.jpeg", .rom = "/home/alissonl/Downloads/Arcade/roms/asteroid.zip"},
    };

    prepararSprites(lista);

    return lista;
}

std::vector<Jogo> carregarComunidade()
{
    // Jogos da comunidade rodam via Wine (.exe). O comando final vira "wine <rom>".
    // Para adicionar um jogo, copie o .exe para a pasta roms/ (ou outro caminho)
    // e coloque a capa em public/assets/capas/.
    std::vector<Jogo> lista = {
        // Exemplo (descomente e ajuste os caminhos para o seu .exe):
        // {.titulo = "Meu Jogo", .emulador = WINE, .imagem = "meujogo.jpg", .rom = "/home/alissonl/Downloads/Arcade/roms/meujogo.exe"},
    };

    prepararSprites(lista);

    return lista;
}