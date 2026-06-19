#include "config.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

const std::string SNES = "retroarch -L /home/projeto/.config/retroarch/cores/snes9x_libretro.so";
const std::string GBA = "retroarch -L /home/projeto/.config/retroarch/cores/mgba_libretro.so";
const std::string NEO = "retroarch -L /home/projeto/.config/retroarch/cores/fbneo_libretro.so";
const std::string PSCX = "retroarch -L /home/projeto/.config/retroarch/cores/snes9x_libretro.so";

std::string lower(std::string data)
{
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return data;
}

std::vector<Jogo> carregarConfiguracoes()
{
    std::vector<Jogo> lista = {
        {.titulo = "Street Fighter 2", .emulador = SNES, .imagem = "sta2.jpg", .rom = "/home/projeto/Downloads/Arcade/roms/sta2.sfc"},
        {.titulo = "Metal Slug 3", .emulador = NEO, .imagem = "mslug3.jpg", .rom = "/home/projeto/Downloads/Arcade/roms/mslug3.neo"},
        {.titulo = "Sonic 3", .emulador = GBA, .imagem = "sonic3.jpg", .rom = "/home/projeto/Downloads/Arcade/roms/sonicadv3.gba"},
        {.titulo = "The king of Fighters", .emulador = NEO, .imagem = "kof98.jpeg", .rom = "/home/projeto/Downloads/Arcade/roms/kof98.neo"},
        {.titulo = "Castlevania", .emulador = GBA, .imagem = "castlevania.jpg", .rom = "/home/projeto/Downloads/Arcade/roms/castlevania.gba"},
        {.titulo = "Super Mario World", .emulador = SNES, .imagem = "smw.jpg", .rom = "/home/projeto/Downloads/Arcade/roms/smw.sfc"},
        {.titulo = "Super Metroid", .emulador = SNES, .imagem = "metroid.jpeg", .rom = "/home/projeto/Downloads/Arcade/roms/metroidt.sfc"},
    };

    float larguraDesejada = 800.f;
    float alturaDesejada = 500.f;

    for (auto &jogo : lista)
    {

        std::string caminhoCapa = "/home/projeto/Downloads/Arcade/public/assets/capas/" + jogo.imagem;

        // ... dentro do loop for (auto &jogo : lista) ...

        if (!jogo.textura.loadFromFile(caminhoCapa))
        {
            std::cout << "Aviso: Capa nao encontrada para " << jogo.titulo << " em " << caminhoCapa << std::endl;

            // SFML 3: cria o sprite a partir da textura (mesmo vazia) e marca como placeholder
            jogo.sprite.emplace(jogo.textura);
            // SFML 3: IntRect agora usa um vetor para o tamanho (ou construtor simplificado)
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

    return lista;
}