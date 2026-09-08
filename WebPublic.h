#pragma once
#include <Arduino.h>

// Landing Page Cientifica do SmartShower 2.0 (100% Offline, sem dependencias externas)
// Contem apresentacao institucional, fundamentacao teorica, fotos reais e simulador.
const char WEB_PUBLIC_HTML[] = R"rawliteral(<!doctype html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<title>SmartShower 2.0 &mdash; Projeto de Conscientiza&ccedil;&atilde;o H&iacute;drica</title>
<style>
:root{
  --bg-deep:#071822;
  --bg-alt:#0B2430;
  --card:#102F3D;
  --card-hi:#153D4D;
  --accent:#38BDF8;
  --turq:#2DD4BF;
  --green:#65C466;
  --text:#F2F7F8;
  --muted:#ADC1CA;
  --border:rgba(255,255,255,.08);
  --warn:#F4B942;
  --radius:16px;
}
*{box-sizing:border-box;margin:0;padding:0}
body{
  font-family:system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",Arial,sans-serif;
  background:linear-gradient(180deg,#071822 0%,#0B2430 55%,#071822 100%);
  color:var(--text);line-height:1.6;font-size:16px;
}
.page{max-width:780px;margin:0 auto;padding:0 18px 48px}
header{
  display:flex;align-items:center;justify-content:space-between;
  padding:18px 0;border-bottom:1px solid var(--border);margin-bottom:28px;
}
.brand{font-size:15px;font-weight:800;color:var(--text);display:flex;align-items:center;gap:6px}
.brand span{color:var(--accent);font-weight:400;font-size:13px}
.badge-header{font-size:11px;color:var(--muted);background:rgba(255,255,255,.05);padding:4px 10px;border-radius:999px;border:1px solid var(--border)}

.hero{text-align:center;padding:24px 0 36px;position:relative}
.eyebrow{
  display:inline-block;font-size:11px;font-weight:800;letter-spacing:.12em;text-transform:uppercase;
  color:var(--turq);background:rgba(45,212,191,.12);padding:4px 14px;border-radius:999px;
  border:1px solid rgba(45,212,191,.3);margin-bottom:16px;
}
h1{font-size:36px;font-weight:900;line-height:1.2;margin-bottom:12px;background:linear-gradient(90deg,#38BDF8,#2DD4BF);-webkit-background-clip:text;-webkit-text-fill-color:transparent}
@media(min-width:600px){h1{font-size:46px}}
.hero-p{font-size:17px;color:var(--muted);max-width:640px;margin:0 auto 20px;line-height:1.5}
.hero-drop{width:64px;height:64px;margin:0 auto 12px;animation:floatDrop 3s ease-in-out infinite}
@keyframes floatDrop{0%,100%{transform:translateY(0)}50%{transform:translateY(-8px)}}

section{margin-bottom:44px}
.sec-title{font-size:24px;font-weight:800;color:var(--text);margin-bottom:14px;display:flex;align-items:center;gap:8px}
.sec-title span{color:var(--accent);font-size:14px;font-weight:700}
.sec-p{color:var(--muted);font-size:15px;margin-bottom:14px;line-height:1.6}

.card{background:var(--card);border:1px solid var(--border);border-radius:var(--radius);padding:20px;margin-bottom:14px}
.card-hi{background:var(--card-hi);border-color:rgba(56,189,248,.25)}
.card-warn{background:rgba(244,185,66,.07);border-color:rgba(244,185,66,.25)}
.card-title{font-size:16px;font-weight:800;color:var(--text);margin-bottom:8px;display:flex;align-items:center;gap:6px}

.grid-2{display:grid;grid-template-columns:1fr;gap:12px}
@media(min-width:600px){.grid-2{grid-template-columns:1fr 1fr}}
.grid-3{display:grid;grid-template-columns:1fr;gap:10px}
@media(min-width:600px){.grid-3{grid-template-columns:repeat(3,1fr)}}

/* Fluxo Hipotese */
.flow-row{display:flex;align-items:center;justify-content:space-between;flex-wrap:wrap;gap:8px;margin:16px 0}
.flow-step{flex:1;min-width:130px;background:var(--card-hi);border:1px solid var(--border);border-radius:12px;padding:12px 10px;text-align:center}
.flow-step strong{display:block;font-size:13px;color:var(--accent);margin-bottom:2px}
.flow-step small{font-size:11px;color:var(--muted)}

/* Timeline */
.timeline{display:grid;grid-template-columns:repeat(auto-fit,minmax(95px,1fr));gap:6px;margin:16px 0}
.time-node{background:var(--bg-alt);border:1px solid var(--border);border-radius:10px;padding:10px 6px;text-align:center;font-size:11px}
.time-node strong{display:block;font-size:12px;color:var(--turq);margin-bottom:2px}

/* Tabela e Comparativo */
.comp-table{width:100%;border-collapse:collapse;font-size:13px;margin:10px 0}
.comp-table th{text-align:left;color:var(--muted);padding:8px 10px;border-bottom:1px solid var(--border);background:rgba(0,0,0,.15)}
.comp-table td{padding:10px;border-bottom:1px solid rgba(255,255,255,.04)}
.comp-table tr:nth-child(even){background:rgba(255,255,255,.01)}

/* Simulador Interativo */
.sim-box{background:var(--card-hi);border:1px solid rgba(56,189,248,.3);border-radius:16px;padding:22px;text-align:center}
.sim-slider-row{margin:14px 0;text-align:left}
.sim-slider-row label{display:flex;justify-content:space-between;font-size:13px;color:var(--muted);font-weight:700;margin-bottom:6px}
input[type=range]{width:100%;height:6px;background:var(--bg-deep);border-radius:999px;outline:none;accent-color:var(--accent)}
.sim-res-val{font-size:44px;font-weight:900;color:var(--accent);line-height:1;margin:12px 0 4px}
.sim-res-sub{font-size:13px;color:var(--muted)}

/* Foto do Prototipo */
.proto-img-wrap{text-align:center;margin:16px 0}
.proto-img{max-width:240px;width:100%;height:auto;border-radius:14px;border:1px solid var(--border);box-shadow:0 12px 28px rgba(0,0,0,.4)}
.proto-cap{font-size:12px;color:var(--muted);margin-top:6px;font-style:italic}

footer{text-align:center;padding:32px 0 16px;border-top:1px solid var(--border);font-size:13px;color:var(--muted)}
@media (prefers-reduced-motion: reduce){*{animation:none !important;transition:none !important}}
</style>
</head>
<body>
<div class="page">
  <header>
    <div class="brand">&#128167; SmartShower 2.0 <span>&bull; Conscientiza&ccedil;&atilde;o H&iacute;drica</span></div>
    <div class="badge-header">Projeto cient&iacute;fico &bull; 2026</div>
  </header>

  <!-- HERO -->
  <div class="hero">
    <div class="hero-drop">
      <svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg">
        <path d="M32 6C32 6 12 30 12 42C12 53.0457 20.9543 62 32 62C43.0457 62 52 53.0457 52 42C52 30 32 6 32 6Z" fill="url(#dropGrad)"/>
        <defs>
          <linearGradient id="dropGrad" x1="12" y1="6" x2="52" y2="62" gradientUnits="userSpaceOnUse">
            <stop stop-color="#38BDF8"/>
            <stop offset="1" stop-color="#2DD4BF"/>
          </linearGradient>
        </defs>
      </svg>
    </div>
    <span class="eyebrow">CI&Ecirc;NCIA &bull; TECNOLOGIA &bull; SUSTENTABILIDADE</span>
    <h1>SmartShower 2.0</h1>
    <h2 style="font-size:18px;font-weight:700;color:var(--text);margin-bottom:12px">Transformando o consumo de &aacute;gua em informa&ccedil;&atilde;o.</h2>
    <p class="hero-p">
      Muitas vezes utilizamos &aacute;gua sem perceber quanto est&aacute; sendo consumido. O SmartShower surgiu da proposta de tornar esse consumo vis&iacute;vel por meio da tecnologia.
    </p>
    <p class="sec-p" style="max-width:640px;margin:0 auto">
      O prot&oacute;tipo mede a passagem da &aacute;gua em tempo real e transforma essa informa&ccedil;&atilde;o em volume, vaz&atilde;o e tempo de utiliza&ccedil;&atilde;o, permitindo que o usu&aacute;rio compreenda melhor seus pr&oacute;prios h&aacute;bitos.
    </p>
  </div>

  <!-- 01 O PROBLEMA -->
  <section>
    <div class="sec-title"><span>01</span> Por que estudar o consumo durante o banho?</div>
    <p class="sec-p">
      A &aacute;gua &eacute; um recurso natural essencial para a vida e para in&uacute;meras atividades humanas. Entretanto, o desperd&iacute;cio continua sendo um desafio ambiental e social.
    </p>
    <p class="sec-p">
      Durante o banho, &eacute; comum que a pessoa saiba aproximadamente quanto tempo permaneceu no chuveiro, mas n&atilde;o saiba quantos litros de &aacute;gua foram realmente utilizados.
    </p>
    <div class="card card-hi">
      <div class="card-title">&#128300; Problema de Pesquisa</div>
      <p style="font-size:14px;color:var(--text);line-height:1.5">
        <em>"Como utilizar a tecnologia para monitorar o consumo de &aacute;gua durante o banho e incentivar a redu&ccedil;&atilde;o do desperd&iacute;cio de forma simples, acess&iacute;vel e eficiente?"</em>
      </p>
    </div>
  </section>

  <!-- 02 HIPOTESE -->
  <section>
    <div class="sec-title"><span>02</span> Nossa Hip&oacute;tese</div>
    <div class="card card-hi">
      <p style="font-size:15px;color:var(--text);line-height:1.5;font-weight:600;text-align:center">
        "Se o consumo de &aacute;gua for apresentado ao usu&aacute;rio em tempo real, essa informa&ccedil;&atilde;o poder&aacute; aumentar sua percep&ccedil;&atilde;o sobre o pr&oacute;prio consumo e estimular h&aacute;bitos mais conscientes."
      </p>
    </div>
    <div class="flow-row">
      <div class="flow-step">
        <strong>1. MEDIR</strong>
        <small>Sensor registra a passagem da &aacute;gua</small>
      </div>
      <div class="flow-step">
        <strong>2. INFORMAR</strong>
        <small>Arduino apresenta litros, tempo e vaz&atilde;o</small>
      </div>
      <div class="flow-step">
        <strong>3. COMPREENDER</strong>
        <small>Visualiza&ccedil;&atilde;o de consumo antes abstrato</small>
      </div>
      <div class="flow-step">
        <strong>4. MUDAR</strong>
        <small>Decis&otilde;es pr&aacute;ticas mais conscientes</small>
      </div>
    </div>
  </section>

  <!-- 03 OBJETIVOS -->
  <section>
    <div class="sec-title"><span>03</span> O que buscamos investigar?</div>
    <div class="card" style="margin-bottom:12px">
      <div class="card-title">&#127919; Objetivo Geral</div>
      <p style="font-size:14px;color:var(--muted)">
        Desenvolver um prot&oacute;tipo capaz de monitorar em tempo real o consumo de &aacute;gua durante o banho e utilizar essa informa&ccedil;&atilde;o como ferramenta de conscientiza&ccedil;&atilde;o.
      </p>
    </div>
    <div class="grid-3">
      <div class="card" style="padding:14px">
        <div style="font-size:12px;font-weight:700;color:var(--turq);margin-bottom:4px">01. Medi&ccedil;&atilde;o</div>
        <p style="font-size:12px;color:var(--muted)">Medir fluxo instant&acirc;neo e volume acumulado.</p>
      </div>
      <div class="card" style="padding:14px">
        <div style="font-size:12px;font-weight:700;color:var(--turq);margin-bottom:4px">02. Exibi&ccedil;&atilde;o</div>
        <p style="font-size:12px;color:var(--muted)">Exibir consumo claro diretamente ao usu&aacute;rio.</p>
      </div>
      <div class="card" style="padding:14px">
        <div style="font-size:12px;font-weight:700;color:var(--turq);margin-bottom:4px">03. Automa&ccedil;&atilde;o</div>
        <p style="font-size:12px;color:var(--muted)">Utilizar Arduino como controlador aut&ocirc;nomo.</p>
      </div>
      <div class="card" style="padding:14px">
        <div style="font-size:12px;font-weight:700;color:var(--turq);margin-bottom:4px">04. Reflex&atilde;o</div>
        <p style="font-size:12px;color:var(--muted)">Estimular reflex&atilde;o ativa sobre o desperd&iacute;cio.</p>
      </div>
      <div class="card" style="padding:14px;grid-column:1/-1">
        <div style="font-size:12px;font-weight:700;color:var(--turq);margin-bottom:4px">05. Sustentabilidade</div>
        <p style="font-size:12px;color:var(--muted)">Demonstrar como tecnologia de baixo custo pode ser aplicada &agrave; preserva&ccedil;&atilde;o h&iacute;drica.</p>
      </div>
    </div>
  </section>

  <!-- 04 COMO FUNCIONA -->
  <section>
    <div class="sec-title"><span>04</span> Como Funciona a Tecnologia?</div>
    <div class="card card-hi" style="text-align:center;padding:16px">
      <div style="font-size:12px;font-weight:800;color:var(--accent);margin-bottom:8px">FLUXO F&Iacute;SICO E L&Oacute;GICO DO PROT&Oacute;TIPO</div>
      <div style="font-size:12px;color:var(--text);line-height:1.8">
        &Aacute;GUA &rarr; SENSOR DE FLUXO &rarr; ROTOR &rarr; SENSOR HALL &rarr; PULSOS EL&Eacute;TRICOS &rarr; ARDUINO &rarr; LITROS / VAZ&Atilde;O / TEMPO &rarr; LCD + WEB APP
      </div>
    </div>
    <p class="sec-p">
      A passagem da &aacute;gua movimenta uma pequena turbina interna no sensor. Esse movimento produz pulsos el&eacute;tricos a cada rota&ccedil;&atilde;o por meio do efeito Hall.
    </p>
    <p class="sec-p">
      O microcontrolador contabiliza esses pulsos e aplica uma constante de calibra&ccedil;&atilde;o para determinar o volume que atravessou o sensor:
    </p>
    <div class="card" style="text-align:center;padding:16px;background:var(--bg-alt)">
      <div style="font-family:monospace;font-size:14px;color:var(--turq);font-weight:700">
        Volume (Litros) = Pulsos Registrados / Pulsos por Litro (fator ~450)
      </div>
      <div style="font-family:monospace;font-size:13px;color:var(--muted);margin-top:6px">
        1 m&sup3; = 1.000 Litros  &bull;  Exemplo: 20 L = 0,020 m&sup3;
      </div>
    </div>
    <div class="card">
      <div class="card-title">&#9878; A Import&acirc;ncia da Calibra&ccedil;&atilde;o Experimental</div>
      <p style="font-size:13px;color:var(--muted)">
        Para transformar corretamente pulsos em litros &eacute; necess&aacute;rio calibrar o sensor utilizando um volume conhecido de teste (como um recipiente graduado de 500 ml ou 1 L). Essa etapa permite comparar o valor calculado pelo sistema com a quantidade real de &aacute;gua coletada, eliminando desvios mec&acirc;nicos.
      </p>
    </div>
  </section>

  <!-- 05 METODO CIENTIFICO -->
  <section>
    <div class="sec-title"><span>05</span> Como Desenvolvemos o Experimento?</div>
    <p class="sec-p">
      O trabalho possui car&aacute;ter bibliogr&aacute;fico e experimental. Foram estudados consumo consciente de &aacute;gua, sustentabilidade, automa&ccedil;&atilde;o e sensores de fluxo. Em seguida foram definidos os componentes, constru&iacute;do o circuito e desenvolvido o firmware.
    </p>
    <div class="timeline">
      <div class="time-node"><strong>1. Pesquisa</strong>Bibliografia</div>
      <div class="time-node"><strong>2. Sele&ccedil;&atilde;o</strong>Componentes</div>
      <div class="time-node"><strong>3. Montagem</strong>Circuito f&iacute;sico</div>
      <div class="time-node"><strong>4. Firmware</strong>C&oacute;digo C++</div>
      <div class="time-node"><strong>5. Calibra&ccedil;&atilde;o</strong>Volumes reais</div>
      <div class="time-node"><strong>6. Testes</strong>Bancada</div>
      <div class="time-node"><strong>7. An&aacute;lise</strong>Resultados</div>
    </div>
  </section>

  <!-- 06 EVOLUCAO 1.0 -> 2.0 -->
  <section>
    <div class="sec-title"><span>06</span> Evolu&ccedil;&atilde;o do Prot&oacute;tipo (1.0 &rarr; 2.0)</div>
    <div class="grid-2">
      <div class="card">
        <div class="card-title">Vers&atilde;o 1.0 &mdash; Valida&ccedil;&atilde;o do Conceito</div>
        <ul style="font-size:13px;color:var(--muted);padding-left:18px;line-height:1.8">
          <li>Arduino Uno tradicional</li>
          <li>Display LCD 16&times;2 I2C</li>
          <li>Sensor de fluxo com turbina</li>
          <li>3 bot&otilde;es f&iacute;sicos</li>
          <li>Temporizador e contador livre</li>
          <li>Dicas e gamifica&ccedil;&atilde;o sonora local</li>
        </ul>
      </div>
      <div class="card card-hi">
        <div class="card-title" style="color:var(--accent)">Vers&atilde;o 2.0 &mdash; Conectividade e Experi&ecirc;ncia</div>
        <ul style="font-size:13px;color:var(--text);padding-left:18px;line-height:1.8">
          <li>Arduino UNO R4 WiFi (32-bit ARM)</li>
          <li>LCD 16&times;2 e bot&otilde;es f&iacute;sicos preservados</li>
          <li>Ponto de Acesso Wi-Fi aut&ocirc;nomo</li>
          <li>Servidor Web embarcado (offline)</li>
          <li>Interface gr&aacute;fica no celular</li>
          <li>C&aacute;lculo com regras da EMBASA</li>
        </ul>
      </div>
    </div>

    <!-- FOTO REAL DO PROTOTIPO OTIMIZADA -->
    <div class="proto-img-wrap">
      <img src="data:image/jpeg;base64,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" alt="Prot&oacute;tipo F&iacute;sico SmartShower em Bancada" class="proto-img">
      <div class="proto-cap">Prot&oacute;tipo f&iacute;sico do SmartShower montado para bancada de testes e feira cient&iacute;fica</div>
    </div>

    <div class="card" style="background:rgba(56,189,248,.06);border-color:rgba(56,189,248,.2)">
      <p style="font-size:13px;color:var(--text)">
        &#128161; <strong>Decis&atilde;o T&eacute;cnica:</strong> A vers&atilde;o 2.0 n&atilde;o elimina o funcionamento f&iacute;sico. O LCD e os bot&otilde;es continuam totalmente operacionais para que o prot&oacute;tipo funcione mesmo sem nenhum celular conectado.
      </p>
    </div>
  </section>

  <!-- 07 RESULTADOS -->
  <section>
    <div class="sec-title"><span>07</span> O que observamos at&eacute; agora?</div>
    <div class="grid-2">
      <div class="card">
        <div class="card-title" style="color:var(--green)">&check; Detec&ccedil;&atilde;o Confi&aacute;vel</div>
        <p style="font-size:13px;color:var(--muted)">O sensor mec&acirc;nico identificou com sensibilidade a passagem e a interrup&ccedil;&atilde;o do fluxo de &aacute;gua.</p>
      </div>
      <div class="card">
        <div class="card-title" style="color:var(--green)">&check; Processamento em Tempo Real</div>
        <p style="font-size:13px;color:var(--muted)">O Arduino calculou a vaz&atilde;o em L/min e o volume total com atualiza&ccedil;&atilde;o a cada ciclo.</p>
      </div>
      <div class="card">
        <div class="card-title" style="color:var(--green)">&check; Visualiza&ccedil;&atilde;o Clara</div>
        <p style="font-size:13px;color:var(--muted)">O consumo p&ocirc;de ser apresentado ao usu&aacute;rio simultaneamente no LCD e na interface web.</p>
      </div>
      <div class="card">
        <div class="card-title" style="color:var(--green)">&check; Viabilidade de Baixo Custo</div>
        <p style="font-size:13px;color:var(--muted)">Componentes eletr&ocirc;nicos acess&iacute;veis demonstraram capacidade para criar ferramentas educativas eficazes.</p>
      </div>
    </div>
    <div class="card card-warn" style="margin-top:10px">
      <p style="font-size:12px;color:var(--warn)">
        &#128300; <strong>Observa&ccedil;&atilde;o Cient&iacute;fica:</strong> Os resultados demonstram a viabilidade t&eacute;cnica do prot&oacute;tipo. Ainda n&atilde;o foi realizado um experimento controlado com amostragem estat&iacute;stica suficiente para quantificar em que medida a visualiza&ccedil;&atilde;o em tempo real altera os h&aacute;bitos a longo prazo.
      </p>
    </div>
  </section>

  <!-- 08 SIMULACAO INTERATIVA -->
  <section>
    <div class="sec-title"><span>08</span> Simula&ccedil;&atilde;o Interativa</div>
    <p class="sec-p">Experimente na pr&aacute;tica: veja como o tempo e a vaz&atilde;o multiplicam o consumo de um banho.</p>
    
    <div class="sim-box">
      <div class="sim-slider-row">
        <label><span>&#9201; Dura&ccedil;&atilde;o do Banho:</span> <span id="lblSimTime" style="color:var(--accent)">8 minutos</span></label>
        <input type="range" id="rngSimTime" min="1" max="20" value="8" oninput="updateSim()">
      </div>
      <div class="sim-slider-row">
        <label><span>&#128167; Vaz&atilde;o do Chuveiro:</span> <span id="lblSimFlow" style="color:var(--turq)">7 L/min (M&eacute;dia chuveiro el&eacute;trico)</span></label>
        <input type="range" id="rngSimFlow" min="2" max="15" value="7" oninput="updateSim()">
      </div>

      <div class="sim-res-val"><span id="lblSimTotalL">56</span> <span style="font-size:20px;font-weight:600">Litros</span></div>
      <div class="sim-res-sub" id="lblSimFormula">C&aacute;lculo: 7 L/min &times; 8 min = 56 Litros (0,056 m&sup3;)</div>
    </div>
  </section>

  <!-- 09 IMPACTO ACUMULADO -->
  <section>
    <div class="sec-title"><span>09</span> Pequenas Mudan&ccedil;as se Acumulam</div>
    <p class="sec-p">Uma mudan&ccedil;a individual pode parecer pequena em um &uacute;nico dia. Mas quando repetida diariamente por uma fam&iacute;lia, o impacto anual se torna gigantesco.</p>

    <div class="card card-hi">
      <div class="grid-3" style="margin-bottom:14px">
        <div>
          <label style="font-size:11px;color:var(--muted);font-weight:700">Economia por banho:</label>
          <div style="font-size:18px;font-weight:800;color:var(--turq)">10 Litros</div>
          <small style="font-size:11px;color:var(--muted)">(apenas ~1,5 min a menos)</small>
        </div>
        <div>
          <label style="font-size:11px;color:var(--muted);font-weight:700">Pessoas na resid&ecirc;ncia:</label>
          <div style="font-size:18px;font-weight:800;color:var(--text)">4 pessoas</div>
          <small style="font-size:11px;color:var(--muted)">(1 banho/dia por pessoa)</small>
        </div>
        <div>
          <label style="font-size:11px;color:var(--muted);font-weight:700">Per&iacute;odo de c&aacute;lculo:</label>
          <div style="font-size:18px;font-weight:800;color:var(--text)">30 dias / 365 dias</div>
        </div>
      </div>

      <div style="background:var(--bg-deep);border-radius:12px;padding:14px;text-align:center;border:1px solid var(--border)">
        <div style="font-size:13px;color:var(--muted);margin-bottom:4px">Volume Poupado:</div>
        <div style="font-size:26px;font-weight:900;color:var(--green)">&asymp; 1.200 Litros por m&ecirc;s</div>
        <div style="font-size:16px;font-weight:700;color:var(--accent);margin-top:2px">&asymp; 14.400 Litros por ano (14,4 m&sup3;)</div>
      </div>
    </div>
  </section>

  <!-- 10 ALEM DO CHUVEIRO -->
  <section>
    <div class="sec-title"><span>10</span> O Princ&iacute;pio &eacute; Maior que o Prot&oacute;tipo</div>
    <p class="sec-p">
      O SmartShower foi desenvolvido para investigar o consumo no banho. No entanto, o princ&iacute;pio fundamental &mdash; medir, interpretar e apresentar dados &mdash; se estende a todas as escalas de utiliza&ccedil;&atilde;o da &aacute;gua:
    </p>
    <table class="comp-table">
      <thead>
        <tr><th>Ambiente</th><th>Aplica&ccedil;&atilde;o Conceitual</th></tr>
      </thead>
      <tbody>
        <tr><td><strong>Resid&ecirc;ncias</strong></td><td>Compreens&atilde;o dos h&aacute;bitos familiares e combate a vazamentos ocultos.</td></tr>
        <tr><td><strong>Condom&iacute;nios</strong></td><td>Acompanhamento setorizado e individualiza&ccedil;&atilde;o de custos.</td></tr>
        <tr><td><strong>Com&eacute;rcio</strong></td><td>Compara&ccedil;&atilde;o de consumo entre per&iacute;odos e efici&ecirc;ncia operacional.</td></tr>
        <tr><td><strong>Grandes Ind&uacute;strias</strong></td><td>Instrumenta&ccedil;&atilde;o, telemetria cont&iacute;nua e metas de sustentabilidade (ESG).</td></tr>
        <tr><td><strong>Agricultura</strong></td><td>Monitoramento de vaz&atilde;o em irriga&ccedil;&atilde;o para evitar desperd&iacute;cio de mananciais.</td></tr>
      </tbody>
    </table>
  </section>

  <!-- 11 LIMITACOES DO PROTOTIPO -->
  <section>
    <div class="sec-title"><span style="color:var(--warn)">11</span> Limita&ccedil;&otilde;es Atuais do Experimento</div>
    <div class="card card-warn">
      <div class="card-title" style="color:var(--warn)">&#9888; O que o prot&oacute;tipo ainda n&atilde;o faz?</div>
      <ul style="font-size:13px;color:var(--text);padding-left:18px;line-height:1.9">
        <li><strong>Calibra&ccedil;&atilde;o em bancada:</strong> O sensor necessita de calibra&ccedil;&atilde;o experimental mais refinada para diferentes press&otilde;es hidr&aacute;ulicas.</li>
        <li><strong>Hist&oacute;rico local:</strong> O hist&oacute;rico de sess&otilde;es em mem&oacute;ria vol&aacute;til &eacute; limitado e zera com o desligamento do microcontrolador.</li>
        <li><strong>Sem bloqueio ativo:</strong> O prot&oacute;tipo atualmente monitora a passagem da &aacute;gua, mas n&atilde;o interrompe fisicamente o fluxo.</li>
        <li><strong>Dispositivo educacional:</strong> Trata-se de um equipamento cient&iacute;fico e demonstrativo, n&atilde;o de um produto hidr&aacute;ulico comercial certificado.</li>
      </ul>
    </div>
  </section>

  <!-- 12 PROXIMOS PASSOS -->
  <section>
    <div class="sec-title"><span>12</span> Para Onde Queremos Evoluir?</div>
    
    <div class="card card-hi" style="margin-bottom:14px">
      <div class="card-title" style="color:var(--accent)">01 &mdash; Acionamento Autom&aacute;tico da &Aacute;gua (V&aacute;lvula Solenoide)</div>
      <p style="font-size:13px;color:var(--text);margin-bottom:10px">
        Um dos objetivos estudados para esta vers&atilde;o foi permitir que o microcontrolador tamb&eacute;m controlasse a passagem da &aacute;gua. A proposta consiste em utilizar uma v&aacute;lvula solenoide acionada por rel&eacute; eletr&ocirc;nico, permitindo interromper o fluxo quando limites de tempo ou volume forem ultrapassados.
      </p>
      <div style="background:var(--bg-deep);border-radius:10px;padding:10px;text-align:center;font-family:monospace;font-size:12px;color:var(--turq)">
        Arduino &rarr; M&oacute;dulo Rel&eacute; / Driver &rarr; V&aacute;lvula Solenoide 12V &rarr; Corte / Libera&ccedil;&atilde;o do Fluxo
      </div>
      <p style="font-size:12px;color:var(--warn);margin-top:8px">
        <em>Nota de rigor: Essa funcionalidade foi investigada durante o desenvolvimento da vers&atilde;o 2.0, mas n&atilde;o foi conclu&iacute;da para a apresenta&ccedil;&atilde;o atual por demandar alimenta&ccedil;&atilde;o externa e isolamento el&eacute;trico na bancada.</em>
      </p>
    </div>

    <div class="grid-2">
      <div class="card">
        <div style="font-size:13px;font-weight:700;color:var(--turq);margin-bottom:4px">02. Calibra&ccedil;&atilde;o Refinada</div>
        <p style="font-size:12px;color:var(--muted)">Curvas de calibra&ccedil;&atilde;o para diferentes temperaturas e press&otilde;es.</p>
      </div>
      <div class="card">
        <div style="font-size:13px;font-weight:700;color:var(--turq);margin-bottom:4px">03. Armazenamento Seguro</div>
        <p style="font-size:12px;color:var(--muted)">Persist&ecirc;ncia de hist&oacute;rico em mem&oacute;ria EEPROM ou cart&atilde;o microSD.</p>
      </div>
      <div class="card">
        <div style="font-size:13px;font-weight:700;color:var(--turq);margin-bottom:4px">04. Estudo com Usu&aacute;rios</div>
        <p style="font-size:12px;color:var(--muted)">Pesquisa emp&iacute;rica sobre a altera&ccedil;&atilde;o de h&aacute;bitos ao longo de semanas.</p>
      </div>
      <div class="card">
        <div style="font-size:13px;font-weight:700;color:var(--turq);margin-bottom:4px">05. Integra&ccedil;&atilde;o em Nuvem</div>
        <p style="font-size:12px;color:var(--muted)">Relat&oacute;rios comparativos peri&oacute;dicos e alertas inteligentes.</p>
      </div>
    </div>
  </section>

  <!-- 13 CONCLUSAO -->
  <section>
    <div class="sec-title"><span>13</span> Conclus&atilde;o</div>
    <div class="card card-hi" style="text-align:center;padding:26px 18px">
      <h3 style="font-size:20px;font-weight:800;color:var(--text);margin-bottom:10px">Tecnologia para tornar o invis&iacute;vel vis&iacute;vel</h3>
      <p class="sec-p" style="max-width:620px;margin:0 auto 16px">
        O SmartShower demonstra como eletr&ocirc;nica, programa&ccedil;&atilde;o e conceitos cient&iacute;ficos podem ser aplicados a um problema ambiental urgente do cotidiano. Mais do que contar litros, o projeto busca transformar o consumo de &aacute;gua em informa&ccedil;&atilde;o compreens&iacute;vel.
      </p>
      <div style="font-size:18px;font-weight:900;color:var(--accent);line-height:1.4">
        "Cada litro pode ser medido.<br>Cada escolha pode fazer diferen&ccedil;a."
      </div>
    </div>
  </section>

  <!-- 14 PROJETO ABERTO & FOOTER -->
  <section>
    <div class="sec-title"><span>14</span> Projeto Aberto</div>
    <div class="card" style="text-align:center;padding:24px 18px">
      <p style="font-size:14px;font-weight:700;color:var(--text);margin-bottom:8px">
        O SmartShower &eacute; um projeto de c&oacute;digo aberto.
      </p>
      <p style="font-size:13px;color:var(--muted);margin-bottom:16px;max-width:520px;margin-left:auto;margin-right:auto">
        O desenvolvimento envolve eletr&ocirc;nica, programa&ccedil;&atilde;o C++, f&iacute;sica aplicada e sustentabilidade. Todo o c&oacute;digo-fonte, esquem&aacute;ticos e documenta&ccedil;&atilde;o podem ser acessados atrav&eacute;s do link abaixo:
      </p>
      <a href="https://github.com/kas-oss/SmartShower" target="_blank" rel="noopener noreferrer" style="display:inline-block;padding:10px 20px;background:rgba(56,189,248,0.12);border:1px solid rgba(56,189,248,0.3);border-radius:10px;font-family:ui-monospace,SFMono-Regular,Menlo,Monaco,Consolas,monospace;font-size:14px;color:var(--accent);font-weight:700;text-decoration:none">
        github.com/kas-oss/SmartShower
      </a>
    </div>
  </section>

  <footer>
    SmartShower 2.0 &bull; Trabalho Cient&iacute;fico de Conscientiza&ccedil;&atilde;o H&iacute;drica &bull; 2026
  </footer>
</div>

<script>
function updateSim(){
  const t = parseInt(document.getElementById('rngSimTime').value) || 8;
  const f = parseInt(document.getElementById('rngSimFlow').value) || 7;
  const total = t * f;
  const m3 = (total / 1000).toFixed(3).replace('.', ',');

  document.getElementById('lblSimTime').textContent = t + ' minutos';
  document.getElementById('lblSimFlow').textContent = f + ' L/min';
  document.getElementById('lblSimTotalL').textContent = total;
  document.getElementById('lblSimFormula').innerHTML = 'C&aacute;lculo: ' + f + ' L/min &times; ' + t + ' min = ' + total + ' Litros (' + m3 + ' m&sup3;)';
}
updateSim();
</script>
</body>
</html>)rawliteral";