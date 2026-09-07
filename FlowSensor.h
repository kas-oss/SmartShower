#pragma once
#include <Arduino.h>
#include "Config.h"

class FlowSensor {
private:
  static volatile unsigned long s_pulsos;
  unsigned long _ultimoCalculoMs;
  unsigned long _pulsosAnteriores;
  float _vazaoAtualLMin;
  float _pulsosPorLitro;
  unsigned long _calibPulsosInicio;
  bool _emCalibracao;

  static void isrContarPulso() {
    s_pulsos++;
  }

public:
  FlowSensor() 
    : _ultimoCalculoMs(0), 
      _pulsosAnteriores(0), 
      _vazaoAtualLMin(0.0f), 
      _pulsosPorLitro(CALIBRACAO_PADRAO_PULSOS_L),
      _calibPulsosInicio(0),
      _emCalibracao(false) {}

  void begin(float pulsosPorLitro = CALIBRACAO_PADRAO_PULSOS_L) {
    _pulsosPorLitro = pulsosPorLitro;
    pinMode(PIN_FLOW_SENSOR, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_FLOW_SENSOR), isrContarPulso, RISING);
    zerar();
  }

  void setCalibracao(float pulsosPorLitro) {
    if (pulsosPorLitro > 10.0f) {
      _pulsosPorLitro = pulsosPorLitro;
    }
  }

  float getCalibracao() const {
    return _pulsosPorLitro;
  }

  void iniciarCalibracao() {
    _calibPulsosInicio = lerPulsos();
    _emCalibracao = true;
  }

  unsigned long getPulsosCalibracao() const {
    if (!_emCalibracao) return 0;
    unsigned long atual = lerPulsos();
    return (atual >= _calibPulsosInicio) ? (atual - _calibPulsosInicio) : 0;
  }

  unsigned long pararCalibracao() {
    unsigned long total = getPulsosCalibracao();
    _emCalibracao = false;
    return total;
  }

  bool isCalibrando() const {
    return _emCalibracao;
  }

  void zerar() {
    noInterrupts();
    s_pulsos = 0;
    interrupts();
    _pulsosAnteriores = 0;
    _vazaoAtualLMin = 0.0f;
    _ultimoCalculoMs = millis();
  }

  unsigned long lerPulsos() const {
    noInterrupts();
    unsigned long val = s_pulsos;
    interrupts();
    return val;
  }

  float calcularLitros() const {
    return (float)lerPulsos() / _pulsosPorLitro;
  }

  float getVazaoLMin() const {
    return _vazaoAtualLMin;
  }

  // Deve ser chamado com frequência para atualizar o cálculo de vazão (L/min)
  void atualizar() {
    unsigned long agora = millis();
    unsigned long deltaMs = agora - _ultimoCalculoMs;

    if (deltaMs < 1000) return;

    unsigned long total = lerPulsos();
    unsigned long deltaPulsos = (total >= _pulsosAnteriores) ? (total - _pulsosAnteriores) : 0;

    if (deltaMs > 0 && _pulsosPorLitro > 0) {
      float litrosDelta = (float)deltaPulsos / _pulsosPorLitro;
      _vazaoAtualLMin = litrosDelta * (60000.0f / (float)deltaMs);
    }

    _pulsosAnteriores = total;
    _ultimoCalculoMs = agora;
  }
};

// Instanciação estática do contador volátil de pulsos
inline volatile unsigned long FlowSensor::s_pulsos = 0;
