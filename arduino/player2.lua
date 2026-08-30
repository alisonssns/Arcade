const int NUM_BOTOES = 11;
const int pinos[NUM_BOTOES] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const char caracteres[NUM_BOTOES] = {'i','j','k','l','u','o','p','b','n','m','y'};

char estadoAnterior[12] = ""; 

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NUM_BOTOES; i++) {
    pinMode(pinos[i], INPUT_PULLUP);
  }
}

void loop() {
  char estadoAtual[12]; 
  int index = 0;
  
  // Lê todos os botões
  for (int i = 0; i < NUM_BOTOES; i++) {
    if (digitalRead(pinos[i]) == LOW) {
      estadoAtual[index] = caracteres[i];
      index++;
    }
  }
  
  if (index == 0) {
    estadoAtual[0] = '-';
    estadoAtual[1] = '\0';
  } else {
    estadoAtual[index] = '\0'; 
  }

  // Só envia se o estado mudou
  if (strcmp(estadoAtual, estadoAnterior) != 0) {
    Serial.println(estadoAtual);
    strcpy(estadoAnterior, estadoAtual);
    delay(15); // Debounce
  }
}