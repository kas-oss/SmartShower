#pragma once
#include <Arduino.h>

// ======================================================
// CONFIGURAÇÃO DE HARDWARE E REDE
// ======================================================

// Pinos dos Botões (Pull-up interno)
const uint8_t PIN_BTN_UP    = 4;
const uint8_t PIN_BTN_DOWN  = 5;
const uint8_t PIN_BTN_OK    = 6;
const uint8_t PIN_BTN_BACK  = 7;

// Pino do Sensor de Fluxo (Efeito Hall)
const uint8_t PIN_FLOW_SENSOR = 2;

// Endereço e dimensões do Display I2C
const uint8_t LCD_I2C_ADDR = 0x27;
const uint8_t LCD_COLS     = 16;
const uint8_t LCD_ROWS     = 2;

// Rede Wi-Fi (Access Point criado pelo Arduino)
const char WIFI_AP_SSID[]     = "SmartShower";
const char WIFI_AP_PASSWORD[] = "smartshower";
const uint16_t HTTP_PORT      = 80;

// Valores padrão
const float CALIBRACAO_PADRAO_PULSOS_L = 450.0f; // Fator do ZJ-S201
const float TARIFA_PADRAO_M3           = 12.50f; // R$ 12,50 por m³ (água + esgoto)

// Capacidade do buffer de histórico
const uint8_t MAX_HISTORICO = 8;

// Tempo de debounce dos botões físicos
const unsigned long DEBOUNCE_DELAY_MS = 180;
