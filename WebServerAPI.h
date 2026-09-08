#pragma once
#include <Arduino.h>
#include <WiFiS3.h>
#include "ShowerSession.h"
#include "WebPublic.h"
#include "WebAdmin.h"

class WebServerAPI {
private:
  WiFiServer& _server;
  ShowerSession& _session;

  void enviarCabecalho(
    WiFiClient& client,
    const char* status,
    const char* contentType,
    int contentLength = -1
  ) {
    client.print("HTTP/1.1 ");
    client.println(status);
    client.print("Content-Type: ");
    client.println(contentType);
    client.println("Access-Control-Allow-Origin: *");
    client.println("Cache-Control: no-store");
    client.println("Connection: close");

    if (contentLength >= 0) {
      client.print("Content-Length: ");
      client.println(contentLength);
    }
    client.println();
  }

  void consumirHeaders(WiFiClient& client) {
    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 1000) {
      if (!client.available()) continue;
      String line = client.readStringUntil('\n');
      line.trim();
      if (line.length() == 0) return;
    }
  }

  String obterParametro(const String& req, const char* nome) {
    String chave = String(nome) + "=";
    int pos = req.indexOf(chave);
    if (pos < 0) return "";
    pos += chave.length();
    int fim = req.indexOf('&', pos);
    if (fim < 0) fim = req.indexOf(' ', pos);
    if (fim < 0) fim = req.length();
    return req.substring(pos, fim);
  }

  void responderStatus(WiFiClient& client) {
    EstadoSistema est = _session.getEstado();
    const char* stateStr = "idle";
    if (est == ESTADO_PAUSADO) stateStr = "paused";
    else if (_session.isAtivo()) stateStr = "running";
    else if (est == ESTADO_FINALIZADO) stateStr = "finished";

    ModoBanho modo = _session.getModo();
    const char* modeStr = (modo == MODO_TIMER) ? "timer" : ((modo == MODO_LITROS) ? "liters" : "free");

    unsigned long sec = _session.getSegundosAtivo();
    unsigned long targetSec = (modo == MODO_TIMER) ? ((unsigned long)_session.getMetaMinutos() * 60UL) : 0UL;
    float targetLitros = (modo == MODO_LITROS) ? _session.getMetaLitros() : 0.0f;

    char litStr[16], flowStr[16], costStr[16], recStr[16], targetLStr[16];
    dtostrf(_session.getLitrosAtuais(), 1, 1, litStr);
    dtostrf(_session.getVazaoLMin(), 1, 1, flowStr);
    dtostrf(_session.getCustoAtual(), 1, 2, costStr);
    dtostrf(_session.getRecorde(), 1, 1, recStr);
    dtostrf(targetLitros, 1, 1, targetLStr);

    char json[450];
    snprintf(
      json, sizeof(json),
      "{\"state\":\"%s\",\"running\":%s,\"mode\":\"%s\","
      "\"seconds\":%lu,\"targetSeconds\":%lu,\"targetLiters\":%s,"
      "\"liters\":%s,\"flow\":%s,\"cost\":%s,"
      "\"score\":%u,\"record\":%s,\"baths\":%u}",
      stateStr,
      _session.isAtivo() ? "true" : "false",
      modeStr,
      sec,
      targetSec,
      targetLStr,
      litStr,
      flowStr,
      costStr,
      _session.getNotaFinal(),
      recStr,
      _session.getTotalBanhos()
    );

    enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(json));
    client.print(json);
  }

  void responderHistorico(WiFiClient& client) {
    uint8_t total = _session.getHistoricoCount();
    char recStr[16];
    dtostrf(_session.getRecorde(), 1, 1, recStr);

    // Início do JSON
    client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n");
    client.print("{\"totalBanhos\":");
    client.print(_session.getTotalBanhos());
    client.print(",\"recorde\":");
    client.print(recStr);
    client.print(",\"itens\":[");

    RegistroBanho reg;
    for (uint8_t i = 0; i < total; i++) {
      if (_session.getRegistroHistorico(i, reg)) {
        if (i > 0) client.print(",");
        char litStr[16], costStr[16];
        dtostrf(reg.litros, 1, 1, litStr);
        dtostrf(reg.custoReais, 1, 2, costStr);

        char item[120];
        snprintf(
          item, sizeof(item),
          "{\"id\":%u,\"mode\":%u,\"seconds\":%u,\"liters\":%s,\"cost\":%s,\"score\":%u}",
          reg.id, reg.modo, reg.duracaoSeg, litStr, costStr, reg.nota
        );
        client.print(item);
      }
    }

    client.print("]}");
  }

  void responderConfig(WiFiClient& client) {
    ConfiguracoesApp cfg = _session.getConfig();
    char tStr[16], pStr[16];
    dtostrf(cfg.tarifaM3, 1, 2, tStr);
    dtostrf(cfg.pulsosPorLitro, 1, 1, pStr);

    char json[120];
    snprintf(json, sizeof(json), "{\"tarifaM3\":%s,\"pulsosL\":%s}", tStr, pStr);
    enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(json));
    client.print(json);
  }

public:
  WebServerAPI(WiFiServer& server, ShowerSession& session)
    : _server(server), _session(session) {}

  void atender() {
    WiFiClient client = _server.available();
    if (!client) return;

    client.setTimeout(250);
    unsigned long inicio = millis();
    while (!client.available() && millis() - inicio < 500) {
      delay(1);
    }

    if (!client.available()) {
      client.stop();
      return;
    }

    String requestLine = client.readStringUntil('\n');
    requestLine.trim();
    consumirHeaders(client);

    // Roteamento
    if (requestLine.startsWith("GET /api/status")) {
      responderStatus(client);
    }
    else if (requestLine.startsWith("POST /api/start")) {
      String modeStr = obterParametro(requestLine, "mode");
      String targetStr = obterParametro(requestLine, "target");

      ModoBanho modo = MODO_TIMER;
      float target = targetStr.toFloat();

      if (modeStr == "liters") {
        modo = MODO_LITROS;
        if (target <= 0) target = 30.0f;
      } else if (modeStr == "free") {
        modo = MODO_LIVRE;
      } else {
        if (target <= 0) target = 5.0f;
      }

      _session.iniciar(modo, target);
      const char ok[] = "{\"ok\":true}";
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(ok));
      client.print(ok);
    }
    else if (requestLine.startsWith("POST /api/pause")) {
      _session.alternarPausa();
      char resp[40];
      snprintf(resp, sizeof(resp), "{\"ok\":true,\"paused\":%s}", _session.isPausado() ? "true" : "false");
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(resp));
      client.print(resp);
    }
    else if (requestLine.startsWith("POST /api/stop")) {
      if (_session.isAtivo()) {
        _session.finalizar(true);
      }
      const char ok[] = "{\"ok\":true}";
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(ok));
      client.print(ok);
    }
    else if (requestLine.startsWith("GET /api/history")) {
      responderHistorico(client);
    }
    else if (requestLine.startsWith("GET /api/config")) {
      responderConfig(client);
    }
    else if (requestLine.startsWith("POST /api/config")) {
      String tariffStr = obterParametro(requestLine, "tariff");
      String pulsesStr = obterParametro(requestLine, "pulses");
      tariffStr.replace(',', '.');
      pulsesStr.replace(',', '.');
      float t = tariffStr.length() > 0 ? tariffStr.toFloat() : -1.0f;
      float p = pulsesStr.length() > 0 ? pulsesStr.toFloat() : -1.0f;
      _session.setConfig(t, p);

      const char ok[] = "{\"ok\":true}";
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(ok));
      client.print(ok);
    }
    else if (requestLine.startsWith("GET /api/calib")) {
      char calibJson[64];
      snprintf(
        calibJson, sizeof(calibJson),
        "{\"calibrating\":%s,\"pulses\":%lu}",
        _session.isCalibrando() ? "true" : "false",
        _session.getPulsosCalibracao()
      );
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(calibJson));
      client.print(calibJson);
    }
    else if (requestLine.startsWith("POST /api/calib/start")) {
      _session.iniciarCalibracao();
      const char ok[] = "{\"ok\":true,\"calibrating\":true}";
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(ok));
      client.print(ok);
    }
    else if (requestLine.startsWith("POST /api/calib/stop")) {
      unsigned long total = _session.pararCalibracao();
      char resp[64];
      snprintf(resp, sizeof(resp), "{\"ok\":true,\"pulses\":%lu}", total);
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(resp));
      client.print(resp);
    }
    else if (requestLine.startsWith("POST /api/calib/reset")) {
      _session.restaurarCalibracaoPadrao();
      char resp[64];
      snprintf(resp, sizeof(resp), "{\"ok\":true,\"pulsesL\":%.1f}", CALIBRACAO_PADRAO_PULSOS_L);
      enviarCabecalho(client, "200 OK", "application/json; charset=utf-8", strlen(resp));
      client.print(resp);
    }
    else if (requestLine.startsWith("GET / ") || requestLine.startsWith("GET /index.html")) {
      enviarCabecalho(client, "200 OK", "text/html; charset=utf-8", strlen(WEB_PUBLIC_HTML));
      size_t total = strlen(WEB_PUBLIC_HTML);
      size_t enviado = 0;
      while (enviado < total && client.connected()) {
        size_t bloco = (total - enviado < 1024) ? (total - enviado) : 1024;
        client.write((const uint8_t*)(WEB_PUBLIC_HTML + enviado), bloco);
        enviado += bloco;
        delay(1);
      }
    }
    else if (requestLine.startsWith("GET /admin")) {
      enviarCabecalho(client, "200 OK", "text/html; charset=utf-8", strlen(WEB_ADMIN_APP));
      size_t total = strlen(WEB_ADMIN_APP);
      size_t enviado = 0;
      while (enviado < total && client.connected()) {
        size_t bloco = (total - enviado < 1024) ? (total - enviado) : 1024;
        client.write((const uint8_t*)(WEB_ADMIN_APP + enviado), bloco);
        enviado += bloco;
        delay(1);
      }
    }
    else if (requestLine.startsWith("GET /favicon.ico")) {
      enviarCabecalho(client, "204 No Content", "text/plain", 0);
    }
    else {
      const char nf[] = "404 Not Found";
      enviarCabecalho(client, "404 Not Found", "text/plain", strlen(nf));
      client.print(nf);
    }

    delay(1);
    client.stop();
  }
};
