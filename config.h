#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <optional>

struct Game
{
    std::string emulador;
    std::string titulo;
    std::string rota;
    std::string imagem;
    std::string rom;
    sf::Texture textura;
    std::optional<sf::Sprite> sprite;
    float escalaBaseX = 1.0f;
    float escalaBaseY = 1.0f;
};

inline const sf::Color COR_FUNDO(15, 20, 25);
inline const sf::Color COR_SIDEBAR(20, 26, 35);
inline const sf::Color COR_DESTAQUE(58, 134, 255);

std::vector<Game> inicializar_jogos(const std::string &pasta_jogos);
std::vector<Game> carregar_comunidade(const std::string &pasta_jogos);
void rodar_jogo(Game jogo);
std::string get_root();

#endif