#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h> // Para usar PROGMEM

// --- CONFIGURAÇÃO DOS PINOS ---
const int pinSensorFluxo = 2; // YF-S201
const int pinBuzzer = 3;

const int btnUp = 5;     // DIREITA (>)
const int btnDown = 4;   // ESQUERDA (<)
const int btnSelect = 6; // OK

// --- INICIALIZAÇÃO DO LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- VARIÁVEIS DE FLUXO ---
volatile unsigned int pulsos = 0;
unsigned long tempoInicio = 0;

// --- ESTADOS DO SISTEMA ---
enum Estado { MENU, AJUSTA_TIMER, TEMPORIZADOR, CONTADOR, FINALIZADO, FRASE, DICA };
Estado estado = MENU;

// --- VARIÁVEIS DE CONTROLE ---
int menuOpcao = 0; // 0=Temporizador, 1=Contador, 2=Dicas
int tempoDefinido = 5;
bool contando = false;       // Flag principal para controlar contagens
bool banhoConcluido = false;

// --- FRASES E DICAS (EM PROGMEM) ---
const char frase0[] PROGMEM = "Cada gota conta!";
const char frase1[] PROGMEM = "Parabens pela economia!";
const char frase2[] PROGMEM = "Banho rapido, planeta feliz!";
const char frase3[] PROGMEM = "Economize agua, salve vidas.";
const char frase4[] PROGMEM = "Menos tempo, mais futuro!";
const char frase5[] PROGMEM = "Sua atitude faz a diferenca!";
const char frase6[] PROGMEM = "O Planeta agradece seu cuidado.";
const char frase7[] PROGMEM = "Pequenos gestos, grandes mudancas.";
const char frase8[] PROGMEM = "Proteger o planeta e simples.";
const char frase9[] PROGMEM = "Use agua com sabedoria.";
const char frase10[] PROGMEM = "Voce inspira mudanca!";

const char* const frases[] PROGMEM = {
  frase0, frase1, frase2, frase3, frase4, frase5, frase6, frase7, frase8, frase9, frase10
};
const int numFrases = sizeof(frases) / sizeof(frases[0]);
int fraseIndex = 0;

const char dica0[] PROGMEM = "Feche o chuveiro ao se ensaboar.";
const char dica1[] PROGMEM = "Reduza o tempo do seu banho.";
const char dica2[] PROGMEM = "Cada minuto economiza ate 12L!";
const char dica3[] PROGMEM = "Banhos frios economizam energia.";
const char dica4[] PROGMEM = "Desligue a agua ao lavar o cabelo.";
const char dica5[] PROGMEM = "Banho rapido, planeta agradece!";
const char dica6[] PROGMEM = "Cada gota faz diferenca!";
const char dica7[] PROGMEM = "Economize agua: use so o necessario.";
const char dica8[] PROGMEM = "Ajude o planeta: diminua o tempo no chuveiro.";
const char dica9[] PROGMEM = "Economize energia com banhos mais curtos.";
const char dica10[] PROGMEM = "Pequenas atitudes, grandes impactos!";

const char* const dicas[] PROGMEM = {
  dica0, dica1, dica2, dica3, dica4, dica5, dica6, dica7, dica8, dica9, dica10
};
const int numDicas = sizeof(dicas) / sizeof(dicas[0]);
// --- DADOS DO USUÁRIO ---
float recordeLitros = 1000.0;
int numBanhos = 0;

// --- FUNÇÕES DE INTERRUPÇÃO ---
void contarPulso() { pulsos++; }

// --- AUXILIARES DE DISPLAY (com buffers) ---
void printComEspacos(const char* texto, int linha, int coluna = 0) {
  lcd.setCursor(coluna, linha);
  char buf[17]; // 16 + null
  strncpy(buf, texto, 16);
  buf[16] = 0;
  lcd.print(buf);
  int espacos = 16 - strlen(buf) - coluna;
  for (int i = 0; i < espacos; i++) lcd.print(" ");
}
void printComEspacos(String texto, int linha, int coluna = 0) {
  printComEspacos(texto.c_str(), linha, coluna);
}

// PROGMEM to RAM helper
void getFraseBuffer(int idx, char* buf, int buflen) {
  strncpy_P(buf, (PGM_P)pgm_read_word(&(frases[idx])), buflen-1);
  buf[buflen-1] = 0;
}
void getDicaBuffer(int idx, char* buf, int buflen) {
  strncpy_P(buf, (PGM_P)pgm_read_word(&(dicas[idx])), buflen-1);
  buf[buflen-1] = 0;
}

// Scroll PROGMEM seguro para Uno/Nano
void scrollTextoPGM(int idx, int linha, int delayScroll, bool frase = true) {
  char buf[65];
  if (frase)
    getFraseBuffer(idx, buf, sizeof(buf));
  else
    getDicaBuffer(idx, buf, sizeof(buf));

  int len = strlen(buf);
  int window = 16;

  if (len <= window) {
    printComEspacos(buf, linha);
    unsigned long tStart = millis();
    while (millis() - tStart < 2000) {
      if (botaoApertado(btnSelect) || botaoApertado(btnUp) || botaoApertado(btnDown)) return;
      delay(10);
    }
    return;
  }

  char textoScroll[81];
  strncpy(textoScroll, buf, 65);
  for (int i = len; i < len + window; i++) textoScroll[i] = ' ';
  textoScroll[len + window] = 0;

  for (int i = 0; i <= len; i++) {
    char pedaco[17];
    strncpy(pedaco, textoScroll + i, window);
    pedaco[window] = 0;
    lcd.setCursor(0, linha);
    lcd.print(pedaco);

    int pause = (i == 0 || i == len) ? 1000 : delayScroll;
    unsigned long tStart = millis();
    while (millis() - tStart < pause) {
      if (botaoApertado(btnSelect) || botaoApertado(btnUp) || botaoApertado(btnDown)) return;
      delay(10);
    }
  }
}

// --- SETUP ---
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pinSensorFluxo, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSensorFluxo), contarPulso, RISING);
  randomSeed(analogRead(A0));
  printComEspacos("Smart Shower v1", 0);
  delay(1500);
  lcd.clear();
}

// --- LOOP PRINCIPAL ---
void loop() {
  switch (estado) {
    case MENU:         mostrarMenu();         break;
    case AJUSTA_TIMER: ajustarTemporizador(); break;
    case TEMPORIZADOR: rodarTemporizador();   break;
    case CONTADOR:     rodarContador();       break;
    case FINALIZADO:   mostrarFinal();        break;
    case FRASE:        mostrarFrase();        break;
    case DICA:         mostrarDica();         break;
  }
}

// --- ESTADOS ---
void mostrarMenu() {
  static unsigned long t0_reset = 0;
  const char* opcoes[3] = {"1. Temporizador", "2. Contador", "3. Dicas"};
  printComEspacos(opcoes[menuOpcao], 1);
  printComEspacos("Selecione < >", 0);

  if (botaoApertado(btnUp)) {
    menuOpcao = (menuOpcao + 1) % 3;
    delay(200);
  }
  if (botaoApertado(btnDown)) {
    menuOpcao = (menuOpcao + 2) % 3;
    delay(200);
  }
  if (botaoApertado(btnSelect)) {
    banhoConcluido = false;
    if (menuOpcao == 0) estado = AJUSTA_TIMER;
    else if (menuOpcao == 1) estado = CONTADOR;
    else estado = DICA;
    contando = false;
    lcd.clear();
    delay(200);
  }
  if (botaoApertado(btnUp) && botaoApertado(btnDown)) {
    if (t0_reset == 0) t0_reset = millis();
    if (millis() - t0_reset > 2000) {
      recordeLitros = 1000.0;
      printComEspacos("Recorde RESETADO!", 0);
      delay(1200);
      lcd.clear();
      t0_reset = 0;
    }
  } else {
    t0_reset = 0;
  }
}

void ajustarTemporizador() {
   static bool primeiraVez = true;
   char buffer[17];
   if (primeiraVez) {
     snprintf(buffer, 17, "Banhos: %d", numBanhos);
     printComEspacos(buffer, 1);

     char recStr[8];
     dtostrf(recordeLitros, 4, 1, recStr);
     char recLinha[17];
     snprintf(recLinha, 17, "Recorde: %s L", recStr);
     printComEspacos(recLinha, 0);

     delay(1400);
     lcd.clear();
     primeiraVez = false;
   }
   snprintf(buffer, 17, "Tempo: %d min", tempoDefinido);
   printComEspacos(buffer, 0);
   printComEspacos("< >   OK", 1);

   if (botaoApertado(btnUp)) {
     if (tempoDefinido < 20) tempoDefinido++;
     delay(200);
   }
   if (botaoApertado(btnDown)) {
     if (tempoDefinido > 1) tempoDefinido--;
     delay(200);
   }
   if (botaoApertado(btnSelect)) {
     contando = true;
     tempoInicio = millis();
     pulsos = 0;
     estado = TEMPORIZADOR;
     lcd.clear();
     delay(200);
     primeiraVez = true;
   }
   if (botaoApertado(btnUp) && botaoApertado(btnDown)) {
     estado = MENU;
     contando = false;
     lcd.clear();
     delay(400);
     primeiraVez = true;
   }
}

void rodarTemporizador() {
  unsigned long tempoDecorrido = (millis() - tempoInicio) / 1000;
  int tempoRestante = tempoDefinido * 60 - tempoDecorrido;

  char linha0[17], linha1[17];
  snprintf(linha0, 17, "Restam: %2d:%02d", tempoRestante/60, tempoRestante%60);

  float litrosAgora = calcularLitros();
  char litrosStr[8];
  dtostrf(litrosAgora, 5, 1, litrosStr);
  snprintf(linha1, 17, "Agua: %s L", litrosStr);

  printComEspacos(linha0, 0);
  printComEspacos(linha1, 1);

  if (tempoRestante <= 0 || botaoApertado(btnSelect)) {
    int nota = calcularNota(litrosAgora);
    tocarEfeitoSonoro(nota);
    contando = false;
    banhoConcluido = true;
    estado = FINALIZADO;
    lcd.clear();
    delay(200);
  }
}

void rodarContador() {
  static unsigned long tempoInicioContador = 0;
  static unsigned long ultimoTempoExibido = 9999;

  // --- Bloco de Inicialização (Executa apenas uma vez) ---
  if (!contando) {
    contando = true; 

    // Mostra as estatísticas iniciais
    char buffer[40];
    snprintf(buffer, 40, "Banhos: %d", numBanhos);
    printComEspacos(buffer, 1);
    char recStr[8];
    dtostrf(recordeLitros, 4, 1, recStr);
    char recLinha[17];
    snprintf(recLinha, 17, "Recorde: %s L", recStr);
    printComEspacos(recLinha, 0);
    delay(1400);
    lcd.clear();

    // Zera os contadores para a nova sessão
    tempoInicioContador = millis();
    pulsos = 0;
    ultimoTempoExibido = 9999; // Reseta o controle de exibição também
  }

  // --- Bloco de Atualização Contínua ---
  unsigned long tempoDecorrido = (millis() - tempoInicioContador) / 1000;

  if (tempoDecorrido != ultimoTempoExibido) {
    ultimoTempoExibido = tempoDecorrido; 
    char buffer[17]; 
    snprintf(buffer, 17, "Tempo: %2lu:%02lu", tempoDecorrido / 60, tempoDecorrido % 60);

    float litrosAgora = calcularLitros();
    char litrosStr[8];
    dtostrf(litrosAgora, 4, 1, litrosStr);
    char linha1[17];
    snprintf(linha1, 17, "Agua: %4s L", litrosStr);

    // Escreve as informações no LCD
    printComEspacos(buffer, 0);
    printComEspacos(linha1, 1);
  }

  // --- Bloco de Verificação de Botões (Responde a qualquer momento) ---
  if (botaoApertado(btnSelect)) {
    int nota = calcularNota(calcularLitros());
    tocarEfeitoSonoro(nota);
    lcd.clear();

    contando = false; 
    banhoConcluido = true;
    
    estado = FINALIZADO;
    delay(200);
  }
  if (botaoApertado(btnUp) && botaoApertado(btnDown)) {
    contando = false;
    estado = MENU;
    lcd.clear();
    delay(400);
  }
}
void mostrarFinal() {
  float litrosFinal = calcularLitros();
  if (banhoConcluido) {
    if (litrosFinal < recordeLitros) recordeLitros = litrosFinal;
    numBanhos++;
    banhoConcluido = false;
  }
  char linha0[17], linha1[17];
  char litrosStr[8];
  dtostrf(litrosFinal, 5, 1, litrosStr); 

  snprintf(linha0, 17, "Final: %s L", litrosStr); 
  String nota = avaliarBanho(litrosFinal);
  strncpy(linha1, nota.c_str(), 16);
  linha1[16] = 0;

  printComEspacos(linha0, 0);
  printComEspacos(linha1, 1);

  if (botaoApertado(btnSelect) || botaoApertado(btnUp) || botaoApertado(btnDown)) {
    fraseIndex = random(numFrases);
    estado = FRASE;
    lcd.clear();
    delay(200);
  }
}

void mostrarFrase() {
  scrollTextoPGM(fraseIndex, 0, 250, true);
  printComEspacos("OK p/Voltar", 1);
  while (!(botaoApertado(btnSelect) || botaoApertado(btnUp) || botaoApertado(btnDown))) {
    delay(50);
  }
  estado = MENU;
  contando = false;
  lcd.clear();
  delay(200);
}

void mostrarDica() {
  int dicaIndex = 0;
  while (true) {
    scrollTextoPGM(dicaIndex, 0, 250, false);
    printComEspacos("OK p/Voltar", 1);

    unsigned long t0 = millis();
    while(millis() - t0 < 1000){
      if (botaoApertado(btnSelect) || botaoApertado(btnUp) || botaoApertado(btnDown)) {
        estado = MENU;
        lcd.clear();
        delay(200);
        return;
      }
      delay(10);
    }
    dicaIndex = (dicaIndex + 1) % numDicas;
    lcd.clear();
  }
}

// --- FUNÇÕES DE CÁLCULO E HARDWARE ---
float calcularLitros() {
  return pulsos / 450.0;
}

String avaliarBanho(float litrosFinal) {
  if (litrosFinal < 7) return "TOP! Nota: 10";
  else if (litrosFinal < 12) return "BOA! Nota: 8";
  else if (litrosFinal < 16) return "P. MELHORAR: 6";
  else return "OPS! Nota: 4";
}

int calcularNota(float litrosFinal) {
  if (litrosFinal < 7) return 10;
  else if (litrosFinal < 12) return 8;
  else if (litrosFinal < 16) return 6;
  else return 4;
}

bool botaoApertado(int pino) {
  return digitalRead(pino) == LOW;
}

void tocarEfeitoSonoro(int nota) {
  if (nota == 10) {
    tone(pinBuzzer, 1000, 100); delay(150);
    tone(pinBuzzer, 1200, 120); delay(180);
    tone(pinBuzzer, 1500, 150); delay(200);
    noTone(pinBuzzer);
  } else if (nota == 8) {
    tone(pinBuzzer, 900, 100); delay(120);
    tone(pinBuzzer, 1200, 120); delay(150);
    noTone(pinBuzzer);
  } else if (nota == 6) {
    tone(pinBuzzer, 700, 160); delay(180);
    noTone(pinBuzzer);
  } else {
    for (int i = 0; i < 2; i++) {
      tone(pinBuzzer, 500, 80); delay(120); noTone(pinBuzzer); delay(80);
    }
  }
}
