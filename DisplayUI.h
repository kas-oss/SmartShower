#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiS3.h>
#include "Config.h"
#include "ShowerSession.h"

enum MenuFisicoOpcao {
  OPT_TIMER = 0,
  OPT_LITROS,
  OPT_LIVRE,
  OPT_REDE,
  OPT_TOTAL
};

class DisplayUI {
private:
  LiquidCrystal_I2C _lcd;
  ShowerSession& _session;

  unsigned long _ultimoBotaoMs;
  uint8_t _menuOpcao;
  bool _emAjuste; // Se está ajustando timer ou litros

  bool botaoPressionado(uint8_t pino) {
    if (millis() - _ultimoBotaoMs < DEBOUNCE_DELAY_MS) return false;
    if (digitalRead(pino) == LOW) {
      _ultimoBotaoMs = millis();
      return true;
    }
    return false;
  }

  void lcdLinha(uint8_t linha, const char* texto) {
    _lcd.setCursor(0, linha);
    char buffer[17];
    strncpy(buffer, texto, 16);
    buffer[16] = '\0';
    _lcd.print(buffer);
    int len = strlen(buffer);
    while (len < 16) {
      _lcd.print(' ');
      len++;
    }
  }

  void mostrarMenu() {
    const char* itens[] = {
      "1.Temporizador",
      "2.Meta Litros",
      "3.Modo Livre",
      "4.Rede WiFi"
    };

    lcdLinha(0, "SmartShower 2.0");
    lcdLinha(1, itens[_menuOpcao]);

    if (botaoPressionado(PIN_BTN_UP)) {
      _menuOpcao = (_menuOpcao + OPT_TOTAL - 1) % OPT_TOTAL;
    }
    if (botaoPressionado(PIN_BTN_DOWN)) {
      _menuOpcao = (_menuOpcao + 1) % OPT_TOTAL;
    }
    if (botaoPressionado(PIN_BTN_OK)) {
      _lcd.clear();
      if (_menuOpcao == OPT_TIMER) {
        _emAjuste = true;
      } else if (_menuOpcao == OPT_LITROS) {
        _emAjuste = true;
      } else if (_menuOpcao == OPT_LIVRE) {
        _session.iniciar(MODO_LIVRE);
      } else if (_menuOpcao == OPT_REDE) {
        mostrarTelaRede(true);
      }
    }
  }

  void mostrarAjuste() {
    char l0[17];
    if (_menuOpcao == OPT_TIMER) {
      snprintf(l0, sizeof(l0), "Tempo: %d min", _session.getMetaMinutos());
      lcdLinha(0, l0);
      lcdLinha(1, "UP/DN  OK:Iniciar");

      if (botaoPressionado(PIN_BTN_UP)) {
        _session.setMetaMinutos(_session.getMetaMinutos() + 1);
      }
      if (botaoPressionado(PIN_BTN_DOWN)) {
        _session.setMetaMinutos(_session.getMetaMinutos() - 1);
      }
      if (botaoPressionado(PIN_BTN_OK)) {
        _emAjuste = false;
        _session.iniciar(MODO_TIMER, _session.getMetaMinutos());
      }
      if (botaoPressionado(PIN_BTN_BACK)) {
        _emAjuste = false;
        _lcd.clear();
      }
    } else if (_menuOpcao == OPT_LITROS) {
      char litStr[8];
      dtostrf(_session.getMetaLitros(), 1, 0, litStr);
      snprintf(l0, sizeof(l0), "Meta: %s L", litStr);
      lcdLinha(0, l0);
      lcdLinha(1, "UP/DN  OK:Iniciar");

      if (botaoPressionado(PIN_BTN_UP)) {
        _session.setMetaLitros(_session.getMetaLitros() + 5.0f);
      }
      if (botaoPressionado(PIN_BTN_DOWN)) {
        _session.setMetaLitros(_session.getMetaLitros() - 5.0f);
      }
      if (botaoPressionado(PIN_BTN_OK)) {
        _emAjuste = false;
        _session.iniciar(MODO_LITROS, _session.getMetaLitros());
      }
      if (botaoPressionado(PIN_BTN_BACK)) {
        _emAjuste = false;
        _lcd.clear();
      }
    }
  }

  void mostrarSessaoAtiva() {
    unsigned long sec = _session.getSegundosAtivo();
    float litros = _session.getLitrosAtuais();
    float custo = _session.getCustoAtual();

    char l0[17];
    char l1[17];
    char litStr[8], custoStr[8];

    dtostrf(litros, 3, 1, litStr);
    dtostrf(custo, 1, 2, custoStr);

    if (_session.getModo() == MODO_TIMER) {
      unsigned long totalSeg = (unsigned long)_session.getMetaMinutos() * 60UL;
      unsigned long rest = (totalSeg > sec) ? (totalSeg - sec) : 0;
      snprintf(l0, sizeof(l0), "Restam %02lu:%02lu", rest / 60, rest % 60);
    } else if (_session.getModo() == MODO_LITROS) {
      float restL = _session.getMetaLitros() - litros;
      if (restL < 0) restL = 0;
      char rStr[8];
      dtostrf(restL, 3, 1, rStr);
      snprintf(l0, sizeof(l0), "Restam %sL", rStr);
    } else {
      snprintf(l0, sizeof(l0), "Tempo %02lu:%02lu", sec / 60, sec % 60);
    }

    snprintf(l1, sizeof(l1), "%sL R$%s", litStr, custoStr);
    lcdLinha(0, l0);
    lcdLinha(1, l1);

    // No banho:
    // UP/DOWN: alternar modo ensaboar (pausa)
    if (botaoPressionado(PIN_BTN_UP) || botaoPressionado(PIN_BTN_DOWN)) {
      _session.alternarPausa();
      _lcd.clear();
    }
    // OK ou BACK: finalizar
    if (botaoPressionado(PIN_BTN_OK) || botaoPressionado(PIN_BTN_BACK)) {
      _session.finalizar(true);
      _lcd.clear();
    }
  }

  void mostrarModoEnsaboar() {
    lcdLinha(0, "MODO ENSABOAR");
    lcdLinha(1, "UP/DN:Retoma OK:Fim");

    if (botaoPressionado(PIN_BTN_UP) || botaoPressionado(PIN_BTN_DOWN)) {
      _session.alternarPausa();
      _lcd.clear();
    }
    if (botaoPressionado(PIN_BTN_OK) || botaoPressionado(PIN_BTN_BACK)) {
      _session.finalizar(true);
      _lcd.clear();
    }
  }

  void mostrarFinalizado() {
    char l0[17], l1[17], litStr[8], custoStr[8];
    dtostrf(_session.getLitrosFinal(), 1, 1, litStr);
    dtostrf(_session.getCustoFinal(), 1, 2, custoStr);

    snprintf(l0, sizeof(l0), "Fim: %sL Nota %d", litStr, _session.getNotaFinal());
    snprintf(l1, sizeof(l1), "R$%s  OK/BACK", custoStr);

    lcdLinha(0, l0);
    lcdLinha(1, l1);

    if (botaoPressionado(PIN_BTN_OK) || botaoPressionado(PIN_BTN_BACK)) {
      _session.cancelar();
      _lcd.clear();
    }
  }

  void mostrarTelaRede(bool bloqueante = false) {
    IPAddress ip = WiFi.localIP();
    char ipBuffer[17];
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

    lcdLinha(0, "WiFi:SmartShower");
    lcdLinha(1, ipBuffer);

    if (bloqueante) {
      delay(2500);
      _lcd.clear();
    }
  }

public:
  DisplayUI(ShowerSession& session)
    : _lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS),
      _session(session),
      _ultimoBotaoMs(0),
      _menuOpcao(0),
      _emAjuste(false) {}

  void begin() {
    _lcd.init();
    _lcd.backlight();

    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_OK, INPUT_PULLUP);
    pinMode(PIN_BTN_BACK, INPUT_PULLUP);

    lcdLinha(0, "SmartShower 2.0");
    lcdLinha(1, "Iniciando...");
  }

  void mostrarBoasVindasWifi(IPAddress ip) {
    char ipBuffer[17];
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    lcdLinha(0, "AP: SmartShower");
    lcdLinha(1, ipBuffer);
    delay(2000);
    _lcd.clear();
  }

  void atualizar() {
    EstadoSistema est = _session.getEstado();

    if (est == ESTADO_IDLE) {
      if (_emAjuste) {
        mostrarAjuste();
      } else {
        mostrarMenu();
      }
    }
    else if (est == ESTADO_BANHO) {
      mostrarSessaoAtiva();
    }
    else if (est == ESTADO_PAUSADO) {
      mostrarModoEnsaboar();
    }
    else if (est == ESTADO_FINALIZADO) {
      mostrarFinalizado();
    }
  }
};
