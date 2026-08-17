g++ controle.cpp -o teclado.exe -std=c++20 -pthread
g++ main.cpp utils.cpp -o Arcade -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system
chmod -x {arquivo} 
chmod -R +rwx ./{pasta}