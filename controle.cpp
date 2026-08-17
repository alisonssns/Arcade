#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/uinput.h>
#include <thread>
#include <atomic>
#include <csignal>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <string>
#include <vector>
#include <cstring>

// Flag global para fechar as threads com segurança via CTRL+C
std::atomic<bool> rodando(true);
std::mutex uinput_mutex; // Evita que os dois arduinos tentem "digitar" no mesmo milissegundo

// Mapa que converte os caracteres recebidos pelo Arduino para Códigos de Teclado do Linux
// (Adicione aqui as teclas que seus Arduinos enviam)
std::unordered_map<char, int> mapa_teclas = {
    {'w', KEY_W}, {'a', KEY_A}, {'s', KEY_S}, {'d', KEY_D},     // Player 1 - Movimento
    {'u', KEY_U}, {'i', KEY_I}, {'o', KEY_O}, {'p', KEY_P},     // Player 1 - Ação
    {'j', KEY_J}, {'k', KEY_K}, {'l', KEY_L}, {'c', KEY_C},     // Player 2 - Movimento
    {'1', KEY_1}, {'2', KEY_2}, {'3', KEY_3}, {'4', KEY_4},     // Player 2 - Ação
    {'5', KEY_5}, {'6', KEY_6}                                  // Fichas / Start
};

// Captura o CTRL+C para sair do loop
void tratar_sinal(int sig) {
    std::cout << "\n[SISTEMA] Encerrando conexões com segurança...\n";
    rodando = false;
}

// Função auxiliar para apertar ou soltar a tecla no kernel
void enviar_tecla(int uinput_fd, int key_code, int valor) {
    struct input_event ie;
    
    // Bloqueia a thread atual por uma fração de milissegundo caso a outra thread esteja usando o teclado
    std::lock_guard<std::mutex> lock(uinput_mutex);
    
    // Envia o evento da tecla (1 = Pressionar, 0 = Soltar)
    ie.type = EV_KEY;
    ie.code = key_code;
    ie.value = valor;
    ie.time.tv_sec = 0; ie.time.tv_usec = 0;
    write(uinput_fd, &ie, sizeof(ie));

    // Sincroniza (Avisa o sistema operacional)
    ie.type = EV_SYN;
    ie.code = SYN_REPORT;
    ie.value = 0;
    write(uinput_fd, &ie, sizeof(ie));
}

// Função que lê linha a linha do Arduino
std::string ler_linha(int fd) {
    std::string linha;
    char c;
    while (rodando) {
        int n = read(fd, &c, 1);
        if (n > 0) {
            if (c == '\n') break;
            if (c != '\r') linha += c;
        } else {
            break; // Timeout (0.1s)
        }
    }
    return linha;
}

// === FUNÇÃO DA THREAD === (Roda em paralelo para cada porta)
void ler_arduino(const std::string& porta, int uinput_fd) {
    int serial_fd = open(porta.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        std::cerr << "[ERRO] Não foi possível abrir " << porta << "\n";
        return;
    }

    // Configuração Serial (115200 Baud Rate)
    struct termios tty;
    tcgetattr(serial_fd, &tty);
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE; tty.c_cflag |= CS8; // 8 bits
    tty.c_cflag &= ~PARENB;                    // Sem paridade
    tty.c_cflag &= ~CSTOPB;                    // 1 stop bit
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Modo RAW
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1; // Timeout de 0.1 segundos
    tcsetattr(serial_fd, TCSANOW, &tty);

    std::cout << "[CONECTADO] " << porta << " operando.\n";

    std::unordered_set<char> teclas_ativas;

    while (rodando) {
        std::string comando = ler_linha(serial_fd);
        if (comando.empty()) continue;

        std::unordered_set<char> estado_atual;
        for (char c : comando) {
            estado_atual.insert(c);
        }

        // Lógica dos Sets (Pressiona as novas)
        for (char t : estado_atual) {
            if (teclas_ativas.find(t) == teclas_ativas.end() && mapa_teclas.count(t)) {
                enviar_tecla(uinput_fd, mapa_teclas[t], 1);
            }
        }

        // Lógica dos Sets (Solta as antigas)
        for (char t : teclas_ativas) {
            if (estado_atual.find(t) == estado_atual.end() && mapa_teclas.count(t)) {
                enviar_tecla(uinput_fd, mapa_teclas[t], 0);
            }
        }

        teclas_ativas = estado_atual;
    }

    // Quando o programa for fechado, solta todas as teclas que esse arduino estava apertando
    for (char t : teclas_ativas) {
        if (mapa_teclas.count(t)) enviar_tecla(uinput_fd, mapa_teclas[t], 0);
    }
    
    close(serial_fd);
    std::cout << "[FECHADO] " << porta << "\n";
}

int main() {
    std::signal(SIGINT, tratar_sinal); // Registra o CTRL+C

    // 1. Prepara o Teclado Virtual no Linux
    int uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0) {
        std::cerr << "Erro uinput! Rode com 'sudo'.\n";
        return 1;
    }

    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    for (const auto& par : mapa_teclas) {
        ioctl(uinput_fd, UI_SET_KEYBIT, par.second); // Cadastra todas as teclas do mapa
    }

    struct uinput_setup usetup = {};
    usetup.id.bustype = BUS_USB;
    strcpy(usetup.name, "Arcade Virtual Controller");
    ioctl(uinput_fd, UI_DEV_SETUP, &usetup);
    ioctl(uinput_fd, UI_DEV_CREATE);

    sleep(1); // Aguarda o PC reconhecer o novo teclado

    // 2. Inicia as Threads para ler os dois Arduinos simultaneamente
    std::string porta1 = "/dev/ttyUSB0";
    std::string porta2 = "/dev/ttyUSB1";

    std::cout << "Iniciando as leituras (CTRL+C para parar)...\n";

    std::thread thread_p1(ler_arduino, porta1, uinput_fd);
    std::thread thread_p2(ler_arduino, porta2, uinput_fd);

    // 3. Aguarda as threads finalizarem
    if (thread_p1.joinable()) thread_p1.join();
    if (thread_p2.joinable()) thread_p2.join();

    // 4. Limpeza final
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);

    std::cout << "Sistema encerrado com sucesso.\n";
    return 0;
}