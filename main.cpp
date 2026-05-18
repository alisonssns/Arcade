#include "config.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

void inicializarJogo(Jogo jogo)
{
    std::string comando = jogo.emulador + " " + jogo.rom;
    int resultado = system(comando.c_str());

    printf("%s \n", comando.c_str());

    if (resultado != 0)
    {
        printf("Não foi possivel abrir o emulador\n");
    }
}

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Projeto Arcade", sf::Style::None);
    sf::Font fonte;

    std::vector<Jogo> listaJogos = carregarConfiguracoes();
    fonte.loadFromFile("./fontes/arial.ttf");
    window.setFramerateLimit(60);

    int selecionado = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::Right && selecionado < listaJogos.size() - 1)
                    selecionado++;
                if (event.key.code == sf::Keyboard::Left && selecionado > 0)
                    selecionado--;
                if (event.key.code == sf::Keyboard::Enter)
                    inicializarJogo(listaJogos[selecionado]);
            }
        }

        float larguraSidebar = 280.f;
        float centroX = larguraSidebar + (window.getSize().x - larguraSidebar) / 2.f;
        float centroY = window.getSize().y / 2.f;
        float sliderY = 400.f;

        window.clear(COR_FUNDO);

        for (int i = 0; i < listaJogos.size(); i++)
        {
            float dist = i - selecionado;
            float offset = dist * 600.f;

            listaJogos[i].sprite.setPosition(centroX + offset, sliderY - 50.f);

            if (i == selecionado)
            {
                listaJogos[i].sprite.setScale(listaJogos[i].escalaBaseX * 1.2f, listaJogos[i].escalaBaseY * 1.2f);
                listaJogos[i].sprite.setColor(sf::Color::White);
            }
            else
            {
                listaJogos[i].sprite.setScale(listaJogos[i].escalaBaseX * 0.8f, listaJogos[i].escalaBaseY * 0.8f);
                listaJogos[i].sprite.setColor(sf::Color(100, 100, 100, 150));
                window.draw(listaJogos[i].sprite);
            }
        }

        window.draw(listaJogos[selecionado].sprite);

        sf::RectangleShape sidebar(sf::Vector2f(larguraSidebar, window.getSize().y));
        sidebar.setFillColor(COR_SIDEBAR);
        window.draw(sidebar);

        sf::Text logo("Projeto Arcade", fonte, 24);
        logo.setPosition(40.f, 50.f);
        logo.setStyle(sf::Text::Bold);
        window.draw(logo);

        sf::Text txtTitulo(listaJogos[selecionado].titulo, fonte, 28);
        txtTitulo.setOrigin(txtTitulo.getGlobalBounds().width / 2, 0);
        txtTitulo.setPosition(centroX, centroY + 180.f);
        window.draw(txtTitulo);

        sf::Text txtInfo(std::to_string(selecionado + 1) + "/" + std::to_string(listaJogos.size()), fonte, 18);
        txtInfo.setOrigin(txtInfo.getGlobalBounds().width / 2, 0);
        txtInfo.setPosition(centroX, centroY + 230.f);
        window.draw(txtInfo);

        sf::Text txtAcao("Pressione (A) para iniciar o jogo", fonte, 18);
        txtAcao.setOrigin(txtAcao.getGlobalBounds().width / 2, 0);
        txtAcao.setPosition(centroX, window.getSize().y - 80.f);
        window.draw(txtAcao);

        window.display();
    }

    return 0;
}