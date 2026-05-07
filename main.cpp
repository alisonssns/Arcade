#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>

void inicializarJogo(std::string jogo)
{
    std::string emulador = "./emuladores/SNES/snes9x-x64.exe";
    // No Windows, o Snes9x usa caminhos de estilo Windows ou relativos
    std::string comando = emulador + " " + jogo;

    std::cout << comando << std::endl;

    int resultado = system(comando.c_str());

    if (resultado != 0)
    {
        std::cerr << "Erro ao abrir o emulador!" << std::endl;
    }
}

int main()
{
    // sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // sf::RenderWindow window(desktop, "Arcade Menu", sf::Style::None);
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arcade Menu");

    sf::Font font;
    font.loadFromFile("./fontes/arial.ttf");

    std::vector<std::string> jogos = {"Street Fighter II", "Metal Slug 3", "Pac-Man", "Donkey Kong"};
    int index = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    index = (index > 0) ? index - 1 : jogos.size() - 1;
                if (event.key.code == sf::Keyboard::Down)
                    index = (index + 1) % jogos.size();
                if (event.key.code == sf::Keyboard::Enter)
                {
                    printf("Iniciando: %s\n", jogos[index].c_str());
                    inicializarJogo("sta2.sfc");
                }
            }
        }

        window.clear(sf::Color::Black);
        for (int i = 0; i < jogos.size(); i++)
        {
            sf::Text text(jogos[i], font, 30);
            text.setPosition(100, 50 + (i * 40));
            text.setFillColor((i == index) ? sf::Color::Yellow : sf::Color::White);
            window.draw(text);
        }
        window.display();
    }
    return 0;
}