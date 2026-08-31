#include "config.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void rodar_jogo(Game jogo)
{
    std::string comando = jogo.emulador + " '" + jogo.rom + "'";
    int resultado = system(comando.c_str());

    printf("%s \n", comando.c_str());

    if (resultado != 0)
    {
        printf("Não foi possivel abrir o emulador\n");
    }
}

int main()
{
    std::string jogos_emulador = "jogos";
    std::string jogos_comunidade = "comunidade";

    sf::VideoMode modoTela = sf::VideoMode::getFullscreenModes()[0];
    sf::RenderWindow window(modoTela, "Projeto Arcade", sf::State::Fullscreen);
    sf::Font fonte;

    std::vector<Game> listaEmulador = inicializar_jogos(jogos_emulador);
    std::vector<Game> listaComunidade = carregar_comunidade(jogos_comunidade);

    for (auto *lista : {&listaEmulador, &listaComunidade})
    {
        for (auto &jogo : *lista)
        {
            if (jogo.textura.getSize().x > 0)
            {
                jogo.sprite->setTexture(jogo.textura);
            }
        }
    }

    if (!fonte.openFromFile(get_root() + "/fontes/font.ttf"))
    {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
    }

    window.setFramerateLimit(60);

    int abaAtual = 0;
    int selecionados[2] = {0, 0};

    while (window.isOpen())
    {
        std::vector<Game> &listaJogos = (abaAtual == 0) ? listaEmulador : listaComunidade;
        int &selecionado = selecionados[abaAtual];

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
                // W / S alternam entre as abas Emulador e Comunidade
                if (keyPressed->scancode == sf::Keyboard::Scan::W)
                    abaAtual = 0;
                if (keyPressed->scancode == sf::Keyboard::Scan::S)
                    abaAtual = 1;
                if (keyPressed->scancode == sf::Keyboard::Scan::D && selecionado < (int)listaJogos.size() - 1)
                    selecionado++;
                if (keyPressed->scancode == sf::Keyboard::Scan::A && selecionado > 0)
                    selecionado--;
                if (keyPressed->scancode == sf::Keyboard::Scan::F && !listaJogos.empty())
                    rodar_jogo(listaJogos[selecionado]);
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

            listaJogos[i].sprite->setPosition({centroX + offset, sliderY - 50.f});

            if (i == selecionado)
            {
                listaJogos[i].sprite->setScale({listaJogos[i].escalaBaseX * 1.2f, listaJogos[i].escalaBaseY * 1.2f});
                listaJogos[i].sprite->setColor(sf::Color::White);
            }
            else
            {
                listaJogos[i].sprite->setScale({listaJogos[i].escalaBaseX * 0.8f, listaJogos[i].escalaBaseY * 0.8f});
                listaJogos[i].sprite->setColor(sf::Color(100, 100, 100, 150));
                window.draw(*listaJogos[i].sprite);
            }
        }

        if (!listaJogos.empty())
            window.draw(*listaJogos[selecionado].sprite);

        sf::RectangleShape sidebar({larguraSidebar, static_cast<float>(window.getSize().y)});
        sidebar.setFillColor(COR_SIDEBAR);
        window.draw(sidebar);

        sf::Text logo(fonte, "Arcade", 24);
        logo.setPosition({40.f, 50.f});
        logo.setStyle(sf::Text::Bold);

        sf::Text emul(fonte, "Emulador", 18);
        emul.setPosition({40.f, 110.f});
        emul.setStyle(sf::Text::Bold);
        emul.setFillColor(abaAtual == 0 ? COR_DESTAQUE : sf::Color::White);

        sf::Text com(fonte, "Comunidade", 18);
        com.setPosition({40.f, 150.f});
        com.setStyle(sf::Text::Bold);
        com.setFillColor(abaAtual == 1 ? COR_DESTAQUE : sf::Color::White);

        window.draw(logo);
        window.draw(com);
        window.draw(emul);

        if (!listaJogos.empty())
        {
            sf::Text txtTitulo(fonte, listaJogos[selecionado].titulo, 28);
            txtTitulo.setOrigin({txtTitulo.getGlobalBounds().size.x / 2.f, 0.f});
            txtTitulo.setPosition({centroX, centroY + 180.f});
            window.draw(txtTitulo);

            sf::Text txtInfo(fonte, std::to_string(selecionado + 1) + "/" + std::to_string(listaJogos.size()), 18);
            txtInfo.setOrigin({txtInfo.getGlobalBounds().size.x / 2.f, 0.f});
            txtInfo.setPosition({centroX, centroY + 230.f});
            window.draw(txtInfo);

            sf::Text txtAcao(fonte, "Pressione o BOTAO VERMELHO para iniciar o jogo", 18);
            txtAcao.setOrigin({txtAcao.getGlobalBounds().size.x / 2.f, 0.f});
            txtAcao.setPosition({centroX, window.getSize().y - 80.f});
            window.draw(txtAcao);
        }
        else
        {
            sf::Text vazio(fonte, "Nenhum jogo nesta aba ainda.", 24);
            vazio.setOrigin({vazio.getGlobalBounds().size.x / 2.f, 0.f});
            vazio.setPosition({centroX, centroY});
            window.draw(vazio);
        }

        window.display();
    }

    return 0;
}