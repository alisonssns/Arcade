#include "config.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

const std::string SNES = "emuladores/SNES/snes9x-x64.exe";
const std::string GBA = "emuladores/GBA/mGBA.exe";
const std::string NEO = "emuladores/fbneo/fbneo.exe";
const std::string PSCX = "emuladores/PCSX/pcsx2-qt.exe";

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
        {.titulo = "Street Fighter 2", .emulador = SNES, .rota = "./emuladores/SNES/", .rom = "sta2.sfc"},
        {.titulo = "Metal Slug 3", .emulador = NEO, .rota = "emuladores/fbneo/fbneo.exe", .rom = "mslug3"},
        {.titulo = "Tekken 3", .emulador = PSCX, .rota = "emuladores/SNES/", .rom = "tekken3"},
        {.titulo = "Sonic 3", .emulador = GBA, .rota = "emuladores/GBA/", .rom = "sonic.gba"},
        {.titulo = "Zelda", .emulador = GBA, .rota = "emuladores/GBA/", .rom = "zelda.gba"}};

    float larguraDesejada = 800.f;
    float alturaDesejada = 500.f;

    for (auto &jogo : lista)
    {
        std::string nomeArquivo = lower(jogo.titulo);
        nomeArquivo.erase(std::remove_if(nomeArquivo.begin(), nomeArquivo.end(), ::isspace), nomeArquivo.end());

        std::string caminhoCapa = "public/assets/capas/" + nomeArquivo + ".jpg";

        if (!jogo.textura.loadFromFile(caminhoCapa))
        {
            std::cout << "Aviso: Capa nao encontrada para " << jogo.titulo << " em " << caminhoCapa << std::endl;

            jogo.sprite.setTextureRect(sf::IntRect(0, 0, 400, 300));
            jogo.sprite.setColor(sf::Color(100, 100, 100));

            jogo.escalaBaseX = larguraDesejada / 400.f;
            jogo.escalaBaseY = alturaDesejada / 300.f;
        }
        else
        {
            jogo.sprite.setTexture(jogo.textura);
            sf::Vector2u tamanhoTextura = jogo.textura.getSize();

            jogo.escalaBaseX = larguraDesejada / tamanhoTextura.x;
            jogo.escalaBaseY = alturaDesejada / tamanhoTextura.y;
        }

        sf::FloatRect bounds = jogo.sprite.getLocalBounds();
        jogo.sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    return lista;
}