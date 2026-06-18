#include "config.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>

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
    // Na SFML 3, o Style::None mudou ligeiramente de escopo (opcional, mas recomendado passar como State)
    sf::RenderWindow window(desktop, "Projeto Arcade", sf::State::Windowed);
    sf::Font fonte;

    std::vector<Jogo> listaJogos = carregarConfiguracoes();

    for (int i = 0; i < listaJogos.size(); i++)
    {
        // Se a textura tem tamanho, significa que o loadFromFile deu certo
        if (listaJogos[i].textura.getSize().x > 0)
        {
            listaJogos[i].sprite->setTexture(listaJogos[i].textura);
        }
    }

    // SFML 3: loadFromFile mudou para openFromFile
    if (!fonte.openFromFile("./fontes/arial.ttf"))
    {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
    }

    window.setFramerateLimit(60);

    int selecionado = 0;

    while (window.isOpen())
    {
        // SFML 3: O novo sistema de eventos com std::optional
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // Verifica se a janela foi fechada
            if (event->is<sf::Event::Closed>())
                window.close();

            // Verifica se uma tecla foi pressionada
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                if (keyPressed->scancode == sf::Keyboard::Scan::Right && selecionado < listaJogos.size() - 1)
                    selecionado++;
                if (keyPressed->scancode == sf::Keyboard::Scan::Left && selecionado > 0)
                    selecionado--;
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter)
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

            // SFML 3: Exige vetor {x, y} em vez de floats separados
            listaJogos[i].sprite->setPosition({centroX + offset, sliderY - 50.f});

            if (i == selecionado)
            {
                listaJogos[i].sprite->setScale({listaJogos[i].escalaBaseX * 1.2f, listaJogos[i].escalaBaseY * 1.2f});
                listaJogos[i].sprite->setColor(sf::Color::White);
            }
            else
            {
                listaJogos[i].sprite->setScale({listaJogos[i].escalaBaseX * 0.8f, listaJogos[i].escalaBaseY * 0.8f});
                // SFML 3: Cores não usam mais construtor com 4 inteiros no formato antigo, usar rgba
                listaJogos[i].sprite->setColor(sf::Color(100, 100, 100, 150));
                window.draw(*listaJogos[i].sprite);
            }
        }

        window.draw(*listaJogos[selecionado].sprite);

        // SFML 3: Construtor do RectangleShape exige vetor com chaves {}
        sf::RectangleShape sidebar({larguraSidebar, static_cast<float>(window.getSize().y)});
        sidebar.setFillColor(COR_SIDEBAR);
        window.draw(sidebar);

        // SFML 3: A fonte vem primeiro, depois a string!
        sf::Text logo(fonte, "Projeto Arcade", 24);
        logo.setPosition({40.f, 50.f});
        // SFML 3: SetStyle também teve o enum alterado para Bold
        logo.setStyle(sf::Text::Bold);
        window.draw(logo);

        sf::Text txtTitulo(fonte, listaJogos[selecionado].titulo, 28);
        // SFML 3: width virou size.x
        txtTitulo.setOrigin({txtTitulo.getGlobalBounds().size.x / 2.f, 0.f});
        txtTitulo.setPosition({centroX, centroY + 180.f});
        window.draw(txtTitulo);

        sf::Text txtInfo(fonte, std::to_string(selecionado + 1) + "/" + std::to_string(listaJogos.size()), 18);
        txtInfo.setOrigin({txtInfo.getGlobalBounds().size.x / 2.f, 0.f});
        txtInfo.setPosition({centroX, centroY + 230.f});
        window.draw(txtInfo);

        sf::Text txtAcao(fonte, "Pressione (A) para iniciar o jogo", 18);
        txtAcao.setOrigin({txtAcao.getGlobalBounds().size.x / 2.f, 0.f});
        txtAcao.setPosition({centroX, window.getSize().y - 80.f});
        window.draw(txtAcao);

        window.display();
    }

    return 0;
}