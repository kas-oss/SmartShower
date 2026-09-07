#pragma once
#include <Arduino.h>

// Modos de operação do banho
enum ModoBanho {
  MODO_TIMER,   // Temporizado por minutos
  MODO_LITROS,  // Meta por volume (Litros)
  MODO_LIVRE    // Sem meta fixa
};

// Estados do sistema
enum EstadoSistema {
  ESTADO_IDLE,        // Pronto / Aguardando no menu
  ESTADO_BANHO,       // Banho ativo
  ESTADO_PAUSADO,     // Modo Ensaboar (pausado)
  ESTADO_FINALIZADO   // Banho concluído com resumo
};

// Registro de um banho no histórico
struct RegistroBanho {
  uint16_t id;
  uint8_t modo;           // 0: Timer, 1: Litros, 2: Livre
  uint16_t duracaoSeg;    // Duração real em segundos
  float litros;           // Litros gastos
  float custoReais;       // Custo estimado em R$
  uint8_t nota;           // Nota de 0 a 10
};

// Configurações persistentes/ajustáveis
struct ConfiguracoesApp {
  float tarifaM3;         // Preço da água em R$/m³ (ex: R$ 12.00/m³)
  float pulsosPorLitro;   // Fator de calibração do sensor (ex: 450.0)
};
