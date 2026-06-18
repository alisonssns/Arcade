#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

struct Jogo
{
    std::string titulo;
    std::string emulador;
    std::string rota;
    std::string imagem;
    std::string rom;
    sf::Texture textura;
    // SFML 3: sf::Sprite exige uma textura no construtor e nao tem default.
    // Guardamos a textura acima e construimos o sprite a partir dela (via emplace).
    std::optional<sf::Sprite> sprite;
    float escalaBaseX = 1.0f;
    float escalaBaseY = 1.0f;
};
// Sem construtor proprio a struct volta a ser um aggregate,
// entao os designated initializers em config.cpp funcionam.

const sf::Color COR_FUNDO(15, 20, 25);
const sf::Color COR_SIDEBAR(20, 26, 35);
const sf::Color COR_DESTAQUE(58, 134, 255);

std::vector<Jogo> carregarConfiguracoes();

#endif