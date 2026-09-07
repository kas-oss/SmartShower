#pragma once
#include <Arduino.h>
#include "Types.h"
#include "Config.h"
#include "FlowSensor.h"

class ShowerSession {
private:
  FlowSensor& _sensor;

  EstadoSistema _estado;
  ModoBanho _modo;
  
  float _metaLitros;
  int _metaMinutos;

  unsigned long _inicioSessaoMs;
  unsigned long _tempoAtivoAcumuladoMs;
  unsigned long _inicioPausaMs;

  float _litrosFinal;
  float _custoFinal;
  uint8_t _notaFinal;
  unsigned long _duracaoFinalSeg;

  unsigned int _totalBanhos;
  float _recordeLitros;

  // Buffer circular para histórico
  RegistroBanho _historico[MAX_HISTORICO];
  uint8_t _historicoCount;
  uint8_t _historicoInicio; // Índice do mais antigo
  uint16_t _proximoId;

  ConfiguracoesApp _config;

public:
  ShowerSession(FlowSensor& sensor)
    : _sensor(sensor),
      _estado(ESTADO_IDLE),
      _modo(MODO_TIMER),
      _metaLitros(30.0f),
      _metaMinutos(5),
      _inicioSessaoMs(0),
      _tempoAtivoAcumuladoMs(0),
      _inicioPausaMs(0),
      _litrosFinal(0.0f),
      _custoFinal(0.0f),
      _notaFinal(10),
      _duracaoFinalSeg(0),
      _totalBanhos(0),
      _recordeLitros(999.0f),
      _historicoCount(0),
      _historicoInicio(0),
      _proximoId(1)
  {
    _config.tarifaM3 = TARIFA_PADRAO_M3;
    _config.pulsosPorLitro = CALIBRACAO_PADRAO_PULSOS_L;
  }

  void begin() {
    _sensor.begin(_config.pulsosPorLitro);
  }

  EstadoSistema getEstado() const { return _estado; }
  ModoBanho getModo() const { return _modo; }
  bool isAtivo() const { return _estado == ESTADO_BANHO || _estado == ESTADO_PAUSADO; }
  bool isPausado() const { return _estado == ESTADO_PAUSADO; }

  int getMetaMinutos() const { return _metaMinutos; }
  float getMetaLitros() const { return _metaLitros; }

  void setMetaMinutos(int min) {
    if (min < 1) min = 1;
    if (min > 60) min = 60;
    _metaMinutos = min;
  }

  void setMetaLitros(float l) {
    if (l < 1.0f) l = 1.0f;
    if (l > 300.0f) l = 300.0f;
    _metaLitros = l;
  }

  ConfiguracoesApp getConfig() const { return _config; }

  void setConfig(float tarifaM3, float pulsosPorLitro) {
    if (tarifaM3 > 0.0f) _config.tarifaM3 = tarifaM3;
    if (pulsosPorLitro > 10.0f) {
      _config.pulsosPorLitro = pulsosPorLitro;
      _sensor.setCalibracao(pulsosPorLitro);
    }
  }

  // Calibração do sensor
  void iniciarCalibracao() { _sensor.iniciarCalibracao(); }
  unsigned long getPulsosCalibracao() const { return _sensor.getPulsosCalibracao(); }
  unsigned long pararCalibracao() { return _sensor.pararCalibracao(); }
  bool isCalibrando() const { return _sensor.isCalibrando(); }
  void restaurarCalibracaoPadrao() { setConfig(-1.0f, CALIBRACAO_PADRAO_PULSOS_L); }

  // Retorna o tempo decorrido ativo (descontando pausas) em segundos
  unsigned long getSegundosAtivo() const {
    if (!isAtivo()) {
      return (_estado == ESTADO_FINALIZADO) ? _duracaoFinalSeg : 0;
    }

    unsigned long ms = _tempoAtivoAcumuladoMs;
    if (_estado == ESTADO_BANHO) {
      ms += (millis() - _inicioSessaoMs);
    }
    return ms / 1000UL;
  }

  float getLitrosAtuais() const {
    if (isAtivo()) {
      return _sensor.calcularLitros();
    }
    return _litrosFinal;
  }

  float getVazaoLMin() const {
    if (_estado == ESTADO_BANHO) {
      return _sensor.getVazaoLMin();
    }
    return 0.0f;
  }

  float calcularCustoAtual(float litros) const {
    // tarifa em R$/m³. 1 m³ = 1000 L.
    return litros * (_config.tarifaM3 / 1000.0f);
  }

  float getCustoAtual() const {
    return calcularCustoAtual(getLitrosAtuais());
  }

  float getRecorde() const {
    return (_recordeLitros >= 990.0f) ? 0.0f : _recordeLitros;
  }

  unsigned int getTotalBanhos() const { return _totalBanhos; }
  uint8_t getNotaFinal() const { return _notaFinal; }
  float getLitrosFinal() const { return _litrosFinal; }
  float getCustoFinal() const { return _custoFinal; }

  uint8_t calcularNota(float litros, unsigned long segundos) {
    // Critério sustentável:
    // Até 10 litros: nota 10
    // Até 18 litros: nota 9
    // Até 25 litros: nota 8
    // Até 35 litros: nota 7
    // Até 45 litros: nota 6
    // Até 60 litros: nota 5
    // Acima: nota 4
    if (litros <= 10.0f) return 10;
    if (litros <= 18.0f) return 9;
    if (litros <= 25.0f) return 8;
    if (litros <= 35.0f) return 7;
    if (litros <= 45.0f) return 6;
    if (litros <= 60.0f) return 5;
    return 4;
  }

  void iniciar(ModoBanho modo, float meta = 0) {
    _modo = modo;
    if (_modo == MODO_TIMER && meta > 0) {
      setMetaMinutos((int)meta);
    } else if (_modo == MODO_LITROS && meta > 0) {
      setMetaLitros(meta);
    }

    _sensor.zerar();
    _tempoAtivoAcumuladoMs = 0;
    _inicioSessaoMs = millis();
    _litrosFinal = 0.0f;
    _custoFinal = 0.0f;
    _notaFinal = 10;
    _duracaoFinalSeg = 0;
    _estado = ESTADO_BANHO;
  }

  void alternarPausa() {
    if (_estado == ESTADO_BANHO) {
      // Pausa (Modo Ensaboar)
      _tempoAtivoAcumuladoMs += (millis() - _inicioSessaoMs);
      _inicioPausaMs = millis();
      _estado = ESTADO_PAUSADO;
    } else if (_estado == ESTADO_PAUSADO) {
      // Retomar banho
      _inicioSessaoMs = millis();
      _estado = ESTADO_BANHO;
    }
  }

  void finalizar(bool registrar = true) {
    if (!isAtivo()) return;

    _duracaoFinalSeg = getSegundosAtivo();
    _litrosFinal = _sensor.calcularLitros();
    _custoFinal = calcularCustoAtual(_litrosFinal);
    _notaFinal = calcularNota(_litrosFinal, _duracaoFinalSeg);

    _estado = ESTADO_FINALIZADO;

    if (registrar && _litrosFinal > 0.05f) {
      _totalBanhos++;
      if (_litrosFinal < _recordeLitros) {
        _recordeLitros = _litrosFinal;
      }
      adicionarHistorico();
    }
  }

  void cancelar() {
    _estado = ESTADO_IDLE;
    _tempoAtivoAcumuladoMs = 0;
    _inicioSessaoMs = 0;
    _sensor.zerar();
  }

  void adicionarHistorico() {
    RegistroBanho reg;
    reg.id = _proximoId++;
    reg.modo = (uint8_t)_modo;
    reg.duracaoSeg = (uint16_t)_duracaoFinalSeg;
    reg.litros = _litrosFinal;
    reg.custoReais = _custoFinal;
    reg.nota = _notaFinal;

    if (_historicoCount < MAX_HISTORICO) {
      uint8_t pos = (_historicoInicio + _historicoCount) % MAX_HISTORICO;
      _historico[pos] = reg;
      _historicoCount++;
    } else {
      // Sobrescreve o mais antigo
      _historico[_historicoInicio] = reg;
      _historicoInicio = (_historicoInicio + 1) % MAX_HISTORICO;
    }
  }

  uint8_t getHistoricoCount() const { return _historicoCount; }

  // Retorna do mais recente (i = 0) até o mais antigo
  bool getRegistroHistorico(uint8_t indexRecente, RegistroBanho& out) const {
    if (indexRecente >= _historicoCount) return false;
    // O mais recente está em: (_historicoInicio + _historicoCount - 1 - indexRecente) % MAX_HISTORICO
    int pos = (int)_historicoInicio + (int)_historicoCount - 1 - (int)indexRecente;
    while (pos < 0) pos += MAX_HISTORICO;
    pos = pos % MAX_HISTORICO;
    out = _historico[pos];
    return true;
  }

  void atualizar() {
    _sensor.atualizar();

    if (_estado == ESTADO_BANHO) {
      // Verifica limite por tempo
      if (_modo == MODO_TIMER) {
        unsigned long limiteSeg = (unsigned long)_metaMinutos * 60UL;
        if (getSegundosAtivo() >= limiteSeg) {
          finalizar(true);
        }
      }
      // Verifica limite por volume (litros)
      else if (_modo == MODO_LITROS) {
        if (_sensor.calcularLitros() >= _metaLitros) {
          finalizar(true);
        }
      }
    }
  }
};
