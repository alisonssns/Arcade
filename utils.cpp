#include "config.h"
#include <filesystem>
#include <unordered_map>
#include <regex>

namespace fs = std::filesystem;

const std::unordered_map<std::string, std::string> emulators = {
    {".sfc", "snesrea9x_libretro.so"},
    {".gba", "mgba_libretro.so"},
    {".nes", "fceumm_libretro.so"},
    {".z64", "mupen64.so"},
    {".zip", "fbneo_libretro.so"},
    {".chd", "flycast_libretro.so"},
};

bool is_img(const std::string ext)
{
    static const std::regex padraoImagem(R"(\.(jpe?g|png)$)", std::regex_constants::icase);
    return std::regex_match(ext, padraoImagem);
}

std::string get_root()
{
    return fs::canonical("/proc/self/exe").parent_path().string();
}

std::string ajustar_titulo(std::string titulo)
{
    size_t pos = titulo.find('_');
    if (pos == std::string::npos)
        return titulo;

    titulo.replace(pos, 1, " ");
    return ajustar_titulo(titulo);
}

void prepararSprites(std::vector<Game> &games)
{
    float larguraDesejada = 800.f;
    float alturaDesejada = 500.f;

    for (Game &jogo : games)
    {

        if (jogo.imagem.empty() || !jogo.textura.loadFromFile(jogo.imagem))
        {
            std::cout << "Aviso: Capa nao encontrada para " << jogo.titulo << " em " << jogo.imagem << std::endl;

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

std::vector<Game> inicializar_jogos(const std::string &pasta_jogos)
{
    std::string root_path = get_root();
    fs::path path(root_path + "/" + pasta_jogos);
    std::cout << "Procurando jogos em: " << path.string() << std::endl;

    std::vector<Game> games;

    if (!fs::exists(path) || !fs::is_directory(path))
    {
        std::cerr << "Aviso: O diretório '" << path.string() << "' não foi encontrado.\n";
        return games;
    }

    for (const auto &entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            Game jogo;
            jogo.titulo = ajustar_titulo(entry.path().filename().string());
            for (const auto &jogos_content : fs::directory_iterator(entry))
            {
                if (!jogos_content.is_regular_file())
                    continue;

                const fs::path arq_path = fs::absolute(jogos_content.path());
                const std::string ext = arq_path.extension().string();

                if (is_img(ext))
                {
                    jogo.imagem = arq_path.string();
                }
                else if (emulators.count(ext))
                {
                    jogo.rom = arq_path.string();

                    std::string core_path = "retroarch -L " + root_path + "/cores/";
                    jogo.emulador = core_path + emulators.at(ext);
                }
            }
            if (!jogo.rom.empty())
            {
                games.push_back(jogo);
            }
        }
    }

    std::cout << "Total de jogos encontrados: " << games.size() << std::endl;

    prepararSprites(games);
    return games;
}

std::vector<Game> carregar_comunidade(const std::string &pasta_jogos)
{
    std::vector<Game> games = {};
    prepararSprites(games);
    return games;
}