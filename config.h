#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct Jogo {
    std::string titulo;
    std::string emulador;
    std::string rota;
    std::string rom;
    sf::Texture textura;
    sf::Sprite sprite;
    float escalaBaseX;
    float escalaBaseY;
};

const sf::Color COR_FUNDO(15, 20, 25);
const sf::Color COR_SIDEBAR(20, 26, 35);
const sf::Color COR_DESTAQUE(58, 134, 255);

std::vector<Jogo> carregarConfiguracoes();

#endif