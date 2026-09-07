/*
  SmartShower 2.0 - Arquitetura API-First (Arduino UNO R4 WiFi)
  
  Módulos:
  - Config.h        : Pinos, constantes de rede e padrões
  - Types.h         : Tipos de dados, estados e structs
  - FlowSensor.h    : Interrupção de hardware e medição de vazão/litros
  - ShowerSession.h : Regras de negócio, custo (R$), modo ensaboar, histórico
  - WebPublic.h     : Landing Page científica de conscientização (GET /)
  - WebAdmin.h      : Painel operacional técnico restrito (GET /admin)
  - WebServerAPI.h  : Servidor HTTP e endpoints da API REST
  - DisplayUI.h     : Controle do LCD 16x2 I2C e 4 botões físicos
*/

#include <Wire.h>
#include <WiFiS3.h>
#include "Config.h"
#include "Types.h"
#include "FlowSensor.h"
#include "ShowerSession.h"
#include "DisplayUI.h"
#include "WebServerAPI.h"

// Componentes centrais
WiFiServer   server(HTTP_PORT);
FlowSensor   sensor;
ShowerSession session(sensor);
DisplayUI    displayUI(session);
WebServerAPI api(server, session);

unsigned long ultimoLcdMs = 0;

void iniciarRede() {
  Serial.println("[WIFI] Criando Access Point...");
  int status = WiFi.beginAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  if (status != WL_AP_LISTENING) {
    Serial.println("[WIFI] Falha ao criar AP. Tentando novamente...");
    delay(1000);
    WiFi.beginAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  }

  server.begin();
  IPAddress ip = WiFi.localIP();
  Serial.print("[WIFI] AP Criado. IP: ");
  Serial.println(ip);

  displayUI.mostrarBoasVindasWifi(ip);
}

void setup() {
  Serial.begin(115200);

  // Inicializa interface física (LCD e botões)
  displayUI.begin();

  // Inicializa sensor e sessão
  session.begin();

  // Inicializa Wi-Fi AP e Servidor Web
  iniciarRede();

  Serial.println("[SISTEMA] SmartShower 2.0 pronto!");
}

void loop() {
  // 1. Processa requisições HTTP da API REST e Web App
  api.atender();

  // 2. Atualiza regras de negócio, limites e medições de água
  session.atualizar();

  // 3. Atualiza Display LCD e lê os 4 botões físicos (a cada ~80ms)
  if (millis() - ultimoLcdMs >= 80) {
    ultimoLcdMs = millis();
    displayUI.atualizar();
  }
}
