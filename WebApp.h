#pragma once
#include <Arduino.h>

// Web App 100% autônomo (HTML, CSS e JS modernos embutidos)
// Funciona offline diretamente conectado ao Wi-Fi do Arduino.
const char WEB_APP[] = R"rawliteral(<!doctype html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<title>SmartShower 2.0</title>
<style>
:root{
  --bg:#08101e;
  --card:#101d30;
  --card-hover:#162740;
  --card-inner:#0b1626;
  --text:#f0f6fc;
  --muted:#8b9eb3;
  --accent:#38bdf8;
  --accent-glow:rgba(56,189,248,.25);
  --ok:#34d399;
  --warn:#fbbf24;
  --danger:#f87171;
  --soap:#c084fc;
  --border:rgba(255,255,255,.09);
  --radius:18px;
}
*{box-sizing:border-box;margin:0;padding:0}
body{
  font-family:system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;
  background:linear-gradient(180deg,#060d19 0%,#0a1628 100%);
  color:var(--text);
  min-height:100vh;
  padding-bottom:30px;
}
.app-container{max-width:560px;margin:0 auto;padding:16px}
header{
  display:flex;align-items:center;justify-content:space-between;
  padding:12px 0 16px;border-bottom:1px solid var(--border);margin-bottom:16px;
}
.brand{font-size:20px;font-weight:800;display:flex;align-items:center;gap:8px}
.brand span{color:var(--accent)}
.badge{
  font-size:11px;font-weight:600;padding:5px 10px;border-radius:999px;
  background:rgba(52,211,153,.15);color:var(--ok);border:1px solid rgba(52,211,153,.3);
  display:flex;align-items:center;gap:5px;
}
.badge.offline{background:rgba(248,113,113,.15);color:var(--danger);border-color:rgba(248,113,113,.3)}
.nav-tabs{
  display:flex;background:var(--card-inner);border:1px solid var(--border);
  border-radius:14px;padding:4px;gap:4px;margin-bottom:16px;
}
.nav-btn{
  flex:1;background:transparent;color:var(--muted);border:none;border-radius:10px;
  padding:9px 2px;font-size:12px;font-weight:700;cursor:pointer;transition:all .2s;white-space:nowrap;
}
.nav-btn.active{background:var(--card);color:var(--text);box-shadow:0 3px 12px rgba(0,0,0,.25)}
.tab-content{display:none}
.tab-content.active{display:block}
.card{
  background:var(--card);border:1px solid var(--border);border-radius:var(--radius);
  padding:20px;margin-bottom:14px;box-shadow:0 8px 24px rgba(0,0,0,.15);
}
.hero{text-align:center;position:relative;overflow:hidden}
.hero::before{
  content:'';position:absolute;top:-40px;left:50%;transform:translateX(-50%);
  width:180px;height:180px;background:radial-gradient(circle,var(--accent-glow) 0%,transparent 70%);
  pointer-events:none;
}
.status-pill{
  display:inline-block;font-size:12px;font-weight:700;letter-spacing:.06em;
  text-transform:uppercase;padding:4px 12px;border-radius:999px;background:var(--card-inner);
  color:var(--muted);margin-bottom:8px;border:1px solid var(--border);
}
.status-pill.active{background:rgba(56,189,248,.15);color:var(--accent);border-color:rgba(56,189,248,.3)}
.status-pill.paused{background:rgba(192,132,252,.15);color:var(--soap);border-color:rgba(192,132,252,.3)}
.liters-display{font-size:62px;font-weight:900;line-height:1;margin:8px 0 2px}
.liters-unit{font-size:15px;color:var(--muted);font-weight:600}
.progress-bar{
  height:8px;background:var(--card-inner);border-radius:999px;overflow:hidden;
  margin-top:16px;border:1px solid var(--border);
}
.progress-fill{height:100%;width:0%;background:var(--accent);transition:width .3s}
.metrics-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:10px;margin-bottom:14px}
.metric-box{background:var(--card);border:1px solid var(--border);border-radius:14px;padding:12px;text-align:center}
.metric-box small{display:block;font-size:11px;color:var(--muted);margin-bottom:4px;font-weight:600}
.metric-box strong{font-size:18px;font-weight:800}
.section-title{font-size:14px;font-weight:700;color:var(--muted);margin-bottom:12px;text-transform:uppercase;letter-spacing:.05em}
.mode-selector{display:grid;grid-template-columns:repeat(3,1fr);gap:8px;margin-bottom:14px}
.mode-btn{
  background:var(--card-inner);border:1px solid var(--border);border-radius:12px;
  padding:10px 4px;font-size:12px;font-weight:700;color:var(--muted);cursor:pointer;
  transition:all .15s;
}
.mode-btn.active{background:var(--accent-glow);color:var(--accent);border-color:var(--accent)}
.chip-group{display:flex;flex-wrap:wrap;gap:6px;margin-bottom:10px}
.chip{
  background:var(--card-inner);border:1px solid var(--border);border-radius:999px;
  padding:6px 12px;font-size:12px;font-weight:700;color:var(--muted);cursor:pointer;
  transition:all .15s;
}
.chip:hover{border-color:var(--accent);color:var(--text)}
.chip.active{background:var(--accent-glow);color:var(--accent);border-color:var(--accent)}
.stepper-row{display:flex;align-items:center;gap:8px;margin-bottom:14px}
.stepper-btn{
  width:44px;height:44px;background:var(--card-inner);border:1px solid var(--border);
  border-radius:10px;color:var(--text);font-size:20px;font-weight:800;cursor:pointer;
  display:flex;align-items:center;justify-content:center;transition:all .15s;user-select:none;
}
.stepper-btn:hover{background:var(--card-hover);border-color:var(--accent)}
.stepper-input{
  flex:1;height:44px;background:var(--card-inner);border:1px solid var(--border);
  border-radius:10px;color:var(--text);padding:0 12px;font-size:16px;font-weight:700;
  text-align:center;outline:none;
}
.stepper-input:focus{border-color:var(--accent)}
.stepper-unit{font-size:13px;color:var(--muted);font-weight:600;min-width:32px}
.input-row{display:flex;align-items:center;gap:10px;margin-bottom:12px;width:100%;box-sizing:border-box}
.input-row label{font-size:13px;color:var(--muted);min-width:70px}
.input-currency{
  display:flex;align-items:center;width:100%;min-width:0;max-width:100%;box-sizing:border-box;
  background:var(--card-inner);border:1px solid var(--border);border-radius:10px;padding:0 12px;gap:8px;
}
.input-currency:focus-within{border-color:var(--accent)}
.input-currency span{font-size:14px;color:var(--muted);font-weight:700;white-space:nowrap}
.input-currency input{
  flex:1;min-width:0;width:100%;height:42px;background:transparent;border:none;color:var(--text);
  font-size:16px;font-weight:700;outline:none;box-sizing:border-box;
}
.cost-helper{
  font-size:12px;color:var(--muted);background:var(--card-inner);border-radius:8px;
  padding:8px 12px;margin-bottom:14px;line-height:1.4;border:1px dashed var(--border);
}
.cost-helper strong{color:var(--accent)}
.btn-grid{display:grid;gap:10px}
button.action{
  height:48px;border-radius:12px;border:none;font-size:14px;font-weight:800;
  cursor:pointer;display:flex;align-items:center;justify-content:center;gap:8px;
  transition:all .15s;
}
.btn-start{background:var(--accent);color:#021020}
.btn-pause{background:rgba(192,132,252,.2);color:#d8b4fe;border:1px solid rgba(192,132,252,.4)}
.btn-stop{background:rgba(248,113,113,.15);color:var(--danger);border:1px solid rgba(248,113,113,.3)}
.btn-primary{background:var(--accent);color:#021020}
.btn-secondary{background:var(--card-inner);color:var(--text);border:1px solid var(--border)}
.btn-secondary:hover{background:var(--card-hover);border-color:var(--accent)}
button:disabled{opacity:.4;cursor:not-allowed}
.history-item{
  display:flex;align-items:center;justify-content:space-between;
  padding:12px 14px;background:var(--card-inner);border:1px solid var(--border);
  border-radius:12px;margin-bottom:8px;font-size:13px;
}
.history-item .tag{
  font-size:10px;font-weight:700;padding:2px 6px;border-radius:6px;
  background:rgba(255,255,255,.08);color:var(--muted);
}
.history-item .score{font-weight:800;font-size:14px;color:var(--ok)}
.history-empty{text-align:center;padding:24px;color:var(--muted);font-size:13px}
.result-box{
  background:rgba(52,211,153,.08);border:1px solid rgba(52,211,153,.25);
  border-radius:14px;padding:16px;text-align:center;margin-bottom:14px;display:none;
}
.result-box.show{display:block}
.result-score{font-size:28px;font-weight:900;color:var(--ok);margin-bottom:4px}
.result-details{font-size:13px;color:var(--muted)}
/* Calibração */
.calib-box{
  background:var(--card-inner);border:1px solid var(--border);border-radius:14px;
  padding:16px;margin-top:12px;
}
.calib-counter{
  font-size:42px;font-weight:900;color:var(--accent);text-align:center;
  margin:10px 0;line-height:1;
}
.calib-pulse-anim{animation:pulseGlow 1.2s infinite alternate}
@keyframes pulseGlow{from{opacity:1;transform:scale(1)}to{opacity:.8;transform:scale(1.03)}}
.calib-result{
  background:rgba(56,189,248,.08);border:1px solid rgba(56,189,248,.25);
  border-radius:12px;padding:12px;margin-top:12px;text-align:center;display:none;
}
.calib-result.show{display:block}
/* Concessionárias & Fatura */
.select-styled{
  width:100%;height:42px;background:var(--card-inner);border:1px solid var(--border);
  border-radius:10px;color:var(--text);font-size:13px;font-weight:700;padding:0 10px;outline:none;
}
.select-styled:focus{border-color:var(--accent)}
.select-styled option{background:var(--card);color:var(--text)}
.bill-hero{
  background:linear-gradient(135deg,rgba(56,189,248,.12) 0%,rgba(16,29,48,1) 100%);
  border:1px solid rgba(56,189,248,.3);text-align:center;padding:18px 14px;border-radius:var(--radius);
  margin-bottom:14px;position:relative;
}
.bill-amount{font-size:36px;font-weight:900;color:var(--ok);line-height:1.1;margin:6px 0}
.bill-subtitle{font-size:12px;color:var(--muted);line-height:1.4}
.bill-badge{
  display:inline-block;font-size:11px;font-weight:700;padding:3px 10px;border-radius:999px;
  background:rgba(56,189,248,.2);color:var(--accent);border:1px solid rgba(56,189,248,.4);margin-bottom:6px;
}
.bill-split{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin:12px 0 0}
.bill-split-box{
  background:var(--card-inner);border:1px solid var(--border);border-radius:10px;padding:8px 6px;text-align:center;
}
.bill-split-box small{display:block;font-size:10px;color:var(--muted);font-weight:600;margin-bottom:2px}
.bill-split-box strong{font-size:14px;color:var(--text);font-weight:800}
.tier-meter-track{height:10px;background:rgba(255,255,255,.07);border-radius:999px;overflow:hidden;margin:10px 0 6px;position:relative}
.tier-meter-fill{height:100%;background:linear-gradient(90deg,var(--ok) 0%,var(--warn) 70%,var(--danger) 100%);transition:width .4s}
.tier-info-row{display:flex;justify-content:space-between;font-size:11px;color:var(--muted)}
.tip-box{
  background:rgba(251,191,36,.08);border:1px solid rgba(251,191,36,.25);border-radius:12px;
  padding:12px;margin-top:10px;font-size:12px;line-height:1.5;color:#fef08a;
}
.tip-box strong{color:#fbbf24}
.bill-table{width:100%;border-collapse:collapse;font-size:11px;margin-top:6px}
.bill-table th{text-align:left;color:var(--muted);padding:6px 4px;border-bottom:1px solid var(--border);font-weight:600}
.bill-table td{padding:7px 4px;border-bottom:1px solid rgba(255,255,255,.05);color:var(--text)}
.bill-table tr.active-tier{background:rgba(56,189,248,.12);color:var(--accent);font-weight:700}
.bill-table .num{text-align:right}
.sub-panel{background:var(--card-inner);border:1px solid var(--border);border-radius:12px;padding:12px;margin-bottom:12px}
.tab-sub-btn{
  flex:1;background:transparent;border:none;color:var(--muted);padding:8px 4px;font-size:12px;
  font-weight:700;border-radius:8px;cursor:pointer;transition:all .15s;
}
.tab-sub-btn.active{background:var(--card);color:var(--text);box-shadow:0 2px 8px rgba(0,0,0,.2)}
</style>
</head>
<body>
<div class="app-container">
  <header>
    <div class="brand">🚿 SmartShower <span>2.0</span></div>
    <div id="statusBadge" class="badge">● Conectando</div>
  </header>

  <nav class="nav-tabs">
    <button class="nav-btn active" onclick="switchTab('tabLive', this)">🚿 Banho</button>
    <button class="nav-btn" onclick="switchTab('tabHistory', this)">📊 Histórico</button>
    <button class="nav-btn" onclick="switchTab('tabBilling', this)">💰 Fatura</button>
    <button class="nav-btn" onclick="switchTab('tabConfig', this)">⚙️ Ajustes</button>
  </nav>

  <!-- ABA 1: BANHO AO VIVO & CONTROLE -->
  <main id="tabLive" class="tab-content active">
    <div id="resultCard" class="result-box">
      <div id="resScore" class="result-score">Nota 10</div>
      <div id="resText" class="result-details">Resumo do banho</div>
    </div>

    <section class="card hero">
      <div id="statePill" class="status-pill">Pronto</div>
      <div class="liters-display"><span id="liveLiters">0.0</span></div>
      <div class="liters-unit">Litros consumidos</div>
      <div class="progress-bar">
        <div id="liveProgress" class="progress-fill"></div>
      </div>
    </section>

    <section class="metrics-grid">
      <div class="metric-box">
        <small>⏱️ Tempo</small>
        <strong id="liveTime">00:00</strong>
      </div>
      <div class="metric-box">
        <small>💧 Vazão</small>
        <strong id="liveFlow">0.0 <span style="font-size:11px;font-weight:normal">L/m</span></strong>
      </div>
      <div class="metric-box">
        <small>💰 Custo</small>
        <strong id="liveCost">R$ 0,00</strong>
      </div>
    </section>

    <section class="card">
      <div class="section-title">Configurar & Iniciar</div>
      
      <div class="mode-selector">
        <button id="btnModeTimer" class="mode-btn active" onclick="selectMode('timer')">⏱️ Tempo</button>
        <button id="btnModeLiters" class="mode-btn" onclick="selectMode('liters')">💧 Litros</button>
        <button id="btnModeFree" class="mode-btn" onclick="selectMode('free')">♾️ Livre</button>
      </div>

      <!-- SELEÇÃO DE TEMPO CUSTOMIZADO -->
      <div id="boxTimer">
        <div class="chip-group">
          <div class="chip" onclick="setTimerMin(1)">1 min</div>
          <div class="chip" onclick="setTimerMin(2)">2 min</div>
          <div class="chip" onclick="setTimerMin(3)">3 min</div>
          <div class="chip active" onclick="setTimerMin(5)">5 min</div>
          <div class="chip" onclick="setTimerMin(8)">8 min</div>
          <div class="chip" onclick="setTimerMin(10)">10 min</div>
          <div class="chip" onclick="setTimerMin(15)">15 min</div>
        </div>
        <div class="stepper-row">
          <button type="button" class="stepper-btn" onclick="stepTimer(-1)">−</button>
          <input type="number" id="inpMinutes" class="stepper-input" value="5" min="1" max="60" oninput="onCustomMinutes()">
          <span class="stepper-unit">min</span>
          <button type="button" class="stepper-btn" onclick="stepTimer(1)">+</button>
        </div>
      </div>

      <!-- SELEÇÃO DE LITROS CUSTOMIZADOS -->
      <div id="boxLiters" style="display:none">
        <div class="chip-group">
          <div class="chip" onclick="setLitersVal(10)">10 L</div>
          <div class="chip" onclick="setLitersVal(15)">15 L</div>
          <div class="chip" onclick="setLitersVal(20)">20 L</div>
          <div class="chip active" onclick="setLitersVal(30)">30 L</div>
          <div class="chip" onclick="setLitersVal(40)">40 L</div>
          <div class="chip" onclick="setLitersVal(50)">50 L</div>
        </div>
        <div class="stepper-row">
          <button type="button" class="stepper-btn" onclick="stepLiters(-5)">−</button>
          <input type="number" id="inpLiters" class="stepper-input" value="30" min="1" max="300" oninput="onCustomLiters()">
          <span class="stepper-unit">L</span>
          <button type="button" class="stepper-btn" onclick="stepLiters(5)">+</button>
        </div>
      </div>

      <div class="btn-grid" style="margin-top:14px">
        <button id="btnStart" class="action btn-start" onclick="startShower()">▶ INICIAR BANHO</button>
        <button id="btnPause" class="action btn-pause" onclick="togglePause()" style="display:none">🧼 MODO ENSABOAR</button>
        <button id="btnStop" class="action btn-stop" onclick="stopShower()" style="display:none">⏹ FINALIZAR BANHO</button>
      </div>
    </section>
  </main>

  <!-- ABA 2: HISTÓRICO -->
  <section id="tabHistory" class="tab-content">
    <div class="card">
      <div class="section-title">Resumo Geral</div>
      <div class="metrics-grid">
        <div class="metric-box">
          <small>Total Banhos</small>
          <strong id="histTotal">0</strong>
        </div>
        <div class="metric-box">
          <small>Melhor Marca</small>
          <strong id="histRecord">0.0 L</strong>
        </div>
        <div class="metric-box">
          <small>Meta Média</small>
          <strong><span id="histAvg">--</span></strong>
        </div>
      </div>
    </div>

    <div class="card">
      <div class="section-title" style="display:flex;justify-content:space-between">
        <span>Últimos Banhos</span>
        <button onclick="fetchHistory()" style="background:none;border:none;color:var(--accent);font-size:12px;cursor:pointer">Atualizar</button>
      </div>
      <div id="historyList">
        <div class="history-empty">Nenhum banho registrado ainda.</div>
      </div>
    </div>
  </section>

  <!-- ABA 3: FATURA & CONCESSIONÁRIA (EMBASA) -->
  <section id="tabBilling" class="tab-content">
    
    <!-- CARD: SELEÇÃO DA CONCESSIONÁRIA & ESGOTO -->
    <div class="card">
      <div class="section-title" style="display:flex;justify-content:space-between;align-items:center">
        <span>Concessionária de Água</span>
        <button type="button" class="btn-secondary" style="height:26px;padding:0 8px;font-size:11px;border-radius:6px" onclick="toggleCustomCompanyForm()">+ Personalizada</button>
      </div>

      <div style="margin-bottom:12px">
        <label style="display:block;font-size:12px;color:var(--muted);margin-bottom:6px;font-weight:600">Empresa / Concessionária Ativa:</label>
        <select id="selConcessionaire" class="select-styled" onchange="onCompanySelectChange()">
          <!-- Preenchido dinamicamente via JavaScript -->
        </select>
        <div id="companyDesc" style="font-size:11px;color:var(--muted);margin-top:4px;line-height:1.3"></div>
      </div>

      <!-- FORMULÁRIO DE CONCESSIONÁRIA PERSONALIZADA (EXPANSÍVEL) -->
      <div id="boxCustomCompany" class="sub-panel" style="display:none">
        <div style="font-size:13px;font-weight:800;color:var(--accent);margin-bottom:10px">➕ Cadastrar Concessionária Personalizada</div>
        <div style="margin-bottom:8px">
          <label style="font-size:11px;color:var(--muted);display:block;margin-bottom:4px">Nome da Concessionária:</label>
          <input type="text" id="custCompName" class="stepper-input" style="height:36px;text-align:left;font-size:13px;width:100%" placeholder="Ex: SAAEB / Sabesp / Copasa">
        </div>
        <div style="display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:8px">
          <div>
            <label style="font-size:11px;color:var(--muted);display:block;margin-bottom:4px">Consumo Mínimo (m³):</label>
            <input type="number" id="custCompMinM3" class="stepper-input" style="height:36px;font-size:13px;width:100%" value="6">
          </div>
          <div>
            <label style="font-size:11px;color:var(--muted);display:block;margin-bottom:4px">Tarifa Mínima Água (R$):</label>
            <input type="number" id="custCompMinCost" class="stepper-input" style="height:36px;font-size:13px;width:100%" value="44.77" step="0.01">
          </div>
        </div>
        <div style="display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:10px">
          <div>
            <label style="font-size:11px;color:var(--muted);display:block;margin-bottom:4px">Taxa Esgoto Padrão (%):</label>
            <input type="number" id="custCompSewage" class="stepper-input" style="height:36px;font-size:13px;width:100%" value="80">
          </div>
          <div>
            <label style="font-size:11px;color:var(--muted);display:block;margin-bottom:4px">Tarifa Faixa 2 (>6 a 10m³):</label>
            <input type="number" id="custCompTier2" class="stepper-input" style="height:36px;font-size:13px;width:100%" value="8.85" step="0.01">
          </div>
        </div>
        <div class="btn-grid" style="grid-template-columns:1fr 1fr;gap:8px">
          <button type="button" class="action btn-primary" style="height:36px;font-size:12px" onclick="saveCustomCompany()">Salvar Empresa</button>
          <button type="button" class="action btn-secondary" style="height:36px;font-size:12px" onclick="toggleCustomCompanyForm(false)">Cancelar</button>
        </div>
      </div>

      <!-- SELETOR DA TAXA DE ESGOTO -->
      <div style="margin-top:10px">
        <label style="display:block;font-size:12px;color:var(--muted);margin-bottom:6px;font-weight:600">Serviço de Esgotamento Sanitário:</label>
        <div class="chip-group" id="sewageChipGroup">
          <div class="chip active" onclick="setSewagePct(80)">80% (Padrão Embasa)</div>
          <div class="chip" onclick="setSewagePct(40)">40% (Decisão Judicial)</div>
          <div class="chip" onclick="setSewagePct(0)">0% (Sem Rede / Fossa)</div>
        </div>
      </div>
    </div>

    <!-- ESTIMATIVA DA CONTA (CARD HERO) -->
    <div class="bill-hero">
      <div id="billCompBadge" class="bill-badge">EMBASA • Residencial</div>
      <div style="font-size:12px;color:var(--muted);font-weight:700;text-transform:uppercase;letter-spacing:.05em">Estimativa da Conta Mensal</div>
      <div id="billTotalAmount" class="bill-amount">R$ 0,00</div>
      <div id="billSubtitle" class="bill-subtitle">Calculado para 0 m³ de consumo no mês</div>

      <div class="bill-split">
        <div class="bill-split-box">
          <small>💧 Água</small>
          <strong id="billWaterVal">R$ 0,00</strong>
        </div>
        <div class="bill-split-box">
          <small>🧪 Esgoto (<span id="billSewagePctText">80%</span>)</small>
          <strong id="billSewageVal">R$ 0,00</strong>
        </div>
      </div>
    </div>

    <!-- TERMÔMETRO DE FAIXA PROGRESSIVA -->
    <div class="card">
      <div class="section-title" style="display:flex;justify-content:space-between;align-items:center">
        <span>Faixa Tarifária Atual</span>
        <span id="tierBadge" style="color:var(--accent);font-size:12px;font-weight:800">Faixa 1 (0 a 6 m³)</span>
      </div>

      <div class="tier-meter-track">
        <div id="tierMeterFill" class="tier-meter-fill" style="width:50%"></div>
      </div>
      <div class="tier-info-row">
        <span id="tierMeterMin">0 m³</span>
        <span id="tierMeterCurrent" style="font-weight:700;color:var(--text)">Consumo: 6.0 m³</span>
        <span id="tierMeterMax">10 m³</span>
      </div>

      <div id="tierAlertBox" class="tip-box">
        💡 <strong>Fique atento:</strong> Faltam <strong>2.0 m³ (2.000 L)</strong> para mudar de faixa tarifária.
      </div>
    </div>

    <!-- SIMULADOR & MEDIÇÃO RESIDENCIAL -->
    <div class="card">
      <div class="section-title">Medição & Perfil da Casa</div>

      <!-- Segmented Control para o modo de simulação -->
      <div style="display:flex;background:var(--card-inner);border:1px solid var(--border);border-radius:10px;padding:3px;gap:4px;margin-bottom:14px">
        <button type="button" id="btnSimModeHouse" class="tab-sub-btn active" onclick="setSimMode('house')">🏡 Perfil da Família</button>
        <button type="button" id="btnSimModeDirect" class="tab-sub-btn" onclick="setSimMode('direct')">🚰 Hidrômetro (m³)</button>
      </div>

      <!-- MODO 1: PERFIL DA FAMÍLIA -->
      <div id="boxSimHouse">
        <div class="input-row">
          <label>Moradores:</label>
          <div class="stepper-row" style="flex:1;margin-bottom:0">
            <button type="button" class="stepper-btn" style="height:36px;width:36px" onclick="stepResidents(-1)">−</button>
            <input type="number" id="inpResidents" class="stepper-input" style="height:36px" value="3" min="1" max="15" oninput="recalcBill()">
            <span class="stepper-unit">pessoas</span>
            <button type="button" class="stepper-btn" style="height:36px;width:36px" onclick="stepResidents(1)">+</button>
          </div>
        </div>

        <div class="input-row">
          <label>Banhos/dia:</label>
          <div class="stepper-row" style="flex:1;margin-bottom:0">
            <button type="button" class="stepper-btn" style="height:36px;width:36px" onclick="stepDailyBaths(-1)">−</button>
            <input type="number" id="inpDailyBaths" class="stepper-input" style="height:36px" value="1" min="1" max="5" oninput="recalcBill()">
            <span class="stepper-unit">por pessoa</span>
            <button type="button" class="stepper-btn" style="height:36px;width:36px" onclick="stepDailyBaths(1)">+</button>
          </div>
        </div>

        <div class="input-row">
          <label>Média Banho:</label>
          <div style="display:flex;align-items:center;gap:6px;flex:1">
            <input type="number" id="inpHouseAvgShower" class="stepper-input" style="height:36px;flex:1" value="32" min="5" max="250" oninput="recalcBill()">
            <span class="stepper-unit">L</span>
            <button type="button" class="btn-secondary" style="height:36px;font-size:11px;padding:0 8px;border-radius:8px;white-space:nowrap" onclick="useSmartShowerAvg()">🔄 Média Real</button>
          </div>
        </div>

        <div style="margin-top:10px">
          <label style="display:block;font-size:11px;color:var(--muted);margin-bottom:4px">Participação do Chuveiro no Total da Casa:</label>
          <div class="chip-group" id="showerWeightGroup">
            <div class="chip" onclick="setShowerWeight(30)">30%</div>
            <div class="chip active" onclick="setShowerWeight(40)">40% (Média Nacional)</div>
            <div class="chip" onclick="setShowerWeight(50)">50%</div>
          </div>
        </div>

        <div class="cost-helper" id="houseSimHelper" style="margin-top:10px">
          🚿 Banhos: <strong>2.880 L/mês</strong> • Casa toda: <strong>~7,20 m³ (7.200 L)</strong>
        </div>
      </div>

      <!-- MODO 2: SIMULAÇÃO DIRETA (M³) -->
      <div id="boxSimDirect" style="display:none">
        <div class="chip-group">
          <div class="chip" onclick="setDirectM3(5)">5 m³ (Mín.)</div>
          <div class="chip" onclick="setDirectM3(8)">8 m³</div>
          <div class="chip active" onclick="setDirectM3(12)">12 m³</div>
          <div class="chip" onclick="setDirectM3(18)">18 m³</div>
          <div class="chip" onclick="setDirectM3(25)">25 m³</div>
          <div class="chip" onclick="setDirectM3(35)">35 m³</div>
        </div>

        <div class="stepper-row" style="margin-top:10px">
          <button type="button" class="stepper-btn" onclick="stepDirectM3(-1)">−</button>
          <input type="number" id="inpDirectM3" class="stepper-input" value="12" min="1" max="500" step="0.5" oninput="recalcBill()">
          <span class="stepper-unit">m³</span>
          <button type="button" class="stepper-btn" onclick="stepDirectM3(1)">+</button>
        </div>
      </div>
    </div>

    <!-- EXTRATO DETALHADO POR FAIXAS -->
    <div class="card">
      <div class="section-title">Extrato Detalhado da Tarifa</div>
      <table class="bill-table">
        <thead>
          <tr>
            <th>Faixa</th>
            <th class="num">Volume</th>
            <th class="num">Tarifa</th>
            <th class="num">Subtotal</th>
          </tr>
        </thead>
        <tbody id="billTableBody">
          <!-- Gerado dinamicamente -->
        </tbody>
      </table>

      <div style="margin-top:14px;border-top:1px solid var(--border);padding-top:12px">
        <button type="button" class="action btn-primary" style="width:100%" onclick="syncTariffWithArduino()">
          ⚡ Sincronizar Tarifa com SmartShower
        </button>
        <div id="billSyncMsg" style="font-size:12px;text-align:center;margin-top:8px;color:var(--ok);display:none">
          Tarifa sincronizada com o chuveiro com sucesso!
        </div>
      </div>
    </div>

  </section>

  <!-- ABA 4: AJUSTES -->
  <section id="tabConfig" class="tab-content">
    <!-- TARIFA DE ÁGUA -->
    <div class="card">
      <div class="section-title">Tarifa de Água & Esgoto</div>
      
      <div style="margin-bottom:12px;display:flex;justify-content:space-between;align-items:center">
        <span style="font-size:12px;color:var(--muted)">Empresa: <strong id="cfgActiveCompName" style="color:var(--accent)">EMBASA</strong></span>
        <button type="button" class="btn-secondary" style="height:26px;padding:0 8px;font-size:11px;border-radius:6px;cursor:pointer" onclick="switchTab('tabBilling')">Regras & Faixas ↗</button>
      </div>

      <div style="margin-bottom:12px">
        <label style="display:block;font-size:13px;color:var(--muted);margin-bottom:6px;font-weight:600">Valor da Tarifa por m³:</label>
        <div class="input-currency">
          <span>R$</span>
          <input type="text" id="cfgTariff" value="12,50" oninput="updateTariffHelper()">
          <span style="font-size:12px;color:var(--muted);white-space:nowrap">/ m³</span>
        </div>
      </div>

      <div id="tariffHelper" class="cost-helper">
        💰 Equivale a <strong>R$ 0,0125</strong> por litro (ou <strong>~R$ 0,38</strong> por banho de 30L).
      </div>

      <button class="action btn-primary" style="width:100%" onclick="saveTariff()">Salvar Tarifa</button>
      <div id="tariffMsg" style="font-size:12px;text-align:center;margin-top:8px;color:var(--ok);display:none">Tarifa salva com sucesso!</div>
    </div>

    <!-- ASSISTENTE DE CALIBRAÇÃO DO SENSOR -->
    <div class="card">
      <div class="section-title">🎯 Calibração do Sensor de Fluxo</div>
      <p style="font-size:12px;color:var(--muted);line-height:1.5;margin-bottom:12px">
        Fator ativo: <strong id="curPulsesDisplay" style="color:var(--accent)">450.0 pulsos/L</strong> (Padrão de fábrica: 450.0)
      </p>

      <div class="calib-box">
        <div style="font-size:13px;font-weight:700;margin-bottom:8px">1. Volume do Recipiente de Teste</div>
        <div class="chip-group">
          <div class="chip active" onclick="setCalibVol(500)">500 ml</div>
          <div class="chip" onclick="setCalibVol(1000)">1000 ml (1 L)</div>
          <div class="chip" onclick="setCalibVol(1500)">1500 ml</div>
          <div class="chip" onclick="setCalibVol(2000)">2000 ml (2 L)</div>
        </div>
        <div class="stepper-row">
          <button type="button" class="stepper-btn" onclick="stepCalibVol(-100)">−</button>
          <input type="number" id="inpCalibVol" class="stepper-input" value="500" min="100" max="10000" step="50" oninput="updateCalibChipSelection()">
          <span class="stepper-unit">ml</span>
          <button type="button" class="stepper-btn" onclick="stepCalibVol(100)">+</button>
        </div>

        <div style="font-size:13px;font-weight:700;margin:12px 0 6px">2. Executar Medição</div>
        <p style="font-size:12px;color:var(--muted);margin-bottom:10px">
          Posicione a garrafa/jarra sob a água. Clique em iniciar, encha até a marca exata e finalize.
        </p>

        <div id="calibActiveArea" style="display:none;text-align:center;margin-bottom:12px">
          <div class="calib-counter calib-pulse-anim" id="calibPulsesCount">0</div>
          <div style="font-size:12px;color:var(--accent);font-weight:600">💧 Contando pulsos do sensor em tempo real...</div>
        </div>

        <div class="btn-grid">
          <button id="btnStartCalib" class="action btn-primary" onclick="startCalibration()">▶ Iniciar Medição de Teste</button>
          <button id="btnStopCalib" class="action btn-stop" onclick="stopCalibration()" style="display:none">⏹ Encheu! Concluir Teste</button>
        </div>

        <!-- RESULTADO DA CALIBRAÇÃO -->
        <div id="calibResultCard" class="calib-result">
          <div style="font-size:14px;font-weight:800;color:var(--ok);margin-bottom:4px">✨ Calibração Calculada!</div>
          <div id="calibResultDetails" style="font-size:13px;color:var(--text);margin-bottom:10px"></div>
          <div class="btn-grid">
            <button class="action btn-primary" onclick="applyCalculatedCalib()">✅ Aplicar Nova Calibração</button>
          </div>
        </div>
      </div>

      <!-- AJUSTE MANUAL E RESTAURAÇÃO -->
      <div style="margin-top:16px;border-top:1px solid var(--border);padding-top:14px">
        <div style="display:flex;align-items:center;gap:8px;margin-bottom:10px;flex-wrap:wrap">
          <label style="font-size:13px;color:var(--muted);white-space:nowrap">Ajuste Manual:</label>
          <input type="number" id="cfgManualPulses" style="flex:1;min-width:70px;max-width:120px;height:38px;padding:0 8px;font-size:13px;box-sizing:border-box" step="1" min="50" max="2500" value="450">
          <span style="font-size:12px;color:var(--muted)">p/L</span>
          <button class="action btn-secondary" style="height:38px;padding:0 14px;font-size:12px;white-space:nowrap" onclick="saveManualPulses()">Aplicar</button>
        </div>
        <button class="action btn-secondary" style="width:100%;height:38px;font-size:12px;color:var(--muted)" onclick="resetFactoryCalib()">🔄 Restaurar Padrão de Fábrica (450.0 pulsos/L)</button>
        <div id="calibSuccessMsg" style="font-size:12px;text-align:center;margin-top:8px;color:var(--ok);display:none">Calibração salva com sucesso!</div>
      </div>
    </div>
  </section>
</div>

<script>
let currentMode = 'timer';
let isRunning = false;
let isPaused = false;
let isCalibrating = false;
let calibTimer = null;
let calculatedFactor = 450.0;
let audioCtx = null;

function switchTab(tabId, el){
  document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(b=>b.classList.remove('active'));
  const targetTab = document.getElementById(tabId);
  if(targetTab) targetTab.classList.add('active');
  let targetBtn = el || (typeof event !== 'undefined' && event && event.currentTarget ? event.currentTarget : null);
  if(!targetBtn) targetBtn = document.querySelector(`.nav-btn[onclick*="${tabId}"]`);
  if(targetBtn) targetBtn.classList.add('active');
  if(tabId==='tabHistory') fetchHistory();
  if(tabId==='tabBilling') initBilling();
  if(tabId==='tabConfig') fetchConfig();
}

function selectMode(mode){
  currentMode = mode;
  document.getElementById('btnModeTimer').classList.toggle('active', mode==='timer');
  document.getElementById('btnModeLiters').classList.toggle('active', mode==='liters');
  document.getElementById('btnModeFree').classList.toggle('active', mode==='free');

  document.getElementById('boxTimer').style.display = (mode==='timer') ? 'block' : 'none';
  document.getElementById('boxLiters').style.display = (mode==='liters') ? 'block' : 'none';
}

function setTimerMin(val){
  document.getElementById('inpMinutes').value = val;
  updateTimerChips(val);
}

function stepTimer(delta){
  const inp = document.getElementById('inpMinutes');
  let val = Math.max(1, Math.min(60, (parseInt(inp.value)||5) + delta));
  inp.value = val;
  updateTimerChips(val);
}

function onCustomMinutes(){
  const inp = document.getElementById('inpMinutes');
  let val = parseInt(inp.value)||1;
  updateTimerChips(val);
}

function updateTimerChips(val){
  document.querySelectorAll('#boxTimer .chip').forEach(c => {
    c.classList.toggle('active', parseInt(c.textContent) === val);
  });
}

function setLitersVal(val){
  document.getElementById('inpLiters').value = val;
  updateLitersChips(val);
}

function stepLiters(delta){
  const inp = document.getElementById('inpLiters');
  let val = Math.max(1, Math.min(300, (parseFloat(inp.value)||30) + delta));
  inp.value = val;
  updateLitersChips(val);
}

function onCustomLiters(){
  const inp = document.getElementById('inpLiters');
  let val = parseFloat(inp.value)||1;
  updateLitersChips(val);
}

function updateLitersChips(val){
  document.querySelectorAll('#boxLiters .chip').forEach(c => {
    c.classList.toggle('active', parseFloat(c.textContent) === val);
  });
}

function initAudio(){
  if(!audioCtx){
    audioCtx = new (window.AudioContext || window.webkitAudioContext)();
  }
  if(audioCtx && audioCtx.state === 'suspended'){
    audioCtx.resume();
  }
}

function beep(freq, duration){
  try{
    initAudio();
    const osc = audioCtx.createOscillator();
    const gain = audioCtx.createGain();
    osc.frequency.value = freq;
    gain.gain.value = 0.1;
    osc.connect(gain);
    gain.connect(audioCtx.destination);
    osc.start();
    gain.gain.exponentialRampToValueAtTime(0.001, audioCtx.currentTime + duration);
    osc.stop(audioCtx.currentTime + duration);
  }catch(e){}
}

function formatSec(sec){
  sec = Math.max(0, parseInt(sec)||0);
  const m = Math.floor(sec/60);
  const s = sec % 60;
  return String(m).padStart(2,'0') + ':' + String(s).padStart(2,'0');
}

async function api(path, opts={}){
  const r = await fetch(path, {cache:'no-store', ...opts});
  if(!r.ok) throw new Error('HTTP '+r.status);
  const ct = r.headers.get('content-type')||'';
  return ct.includes('application/json') ? r.json() : r.text();
}

async function startShower(){
  initAudio();
  let target = 0;
  if(currentMode === 'timer') target = document.getElementById('inpMinutes').value;
  if(currentMode === 'liters') target = document.getElementById('inpLiters').value;

  await api('/api/start?mode='+currentMode+'&target='+target, {method:'POST'});
  beep(880, 0.12);
  poll();
}

async function togglePause(){
  initAudio();
  await api('/api/pause', {method:'POST'});
  beep(600, 0.1);
  poll();
}

async function stopShower(){
  initAudio();
  await api('/api/stop', {method:'POST'});
  beep(440, 0.15);
  poll();
}

async function fetchHistory(){
  try{
    const data = await api('/api/history');
    document.getElementById('histTotal').textContent = data.totalBanhos || 0;
    document.getElementById('histRecord').textContent = (data.recorde ? Number(data.recorde).toFixed(1) : '0.0') + ' L';

    const list = document.getElementById('historyList');
    if(!data.itens || data.itens.length === 0){
      list.innerHTML = '<div class="history-empty">Nenhum banho registrado ainda.</div>';
      return;
    }

    let html = '';
    let totalL = 0;
    data.itens.forEach(item => {
      totalL += Number(item.liters);
      const modoNome = item.mode === 0 ? 'Tempo' : (item.mode === 1 ? 'Litros' : 'Livre');
      html += `
        <div class="history-item">
          <div>
            <strong>Banho #${item.id}</strong> 
            <span class="tag">${modoNome}</span>
            <div style="color:var(--muted);font-size:11px;margin-top:2px">
              ⏱️ ${formatSec(item.seconds)} • 💧 ${Number(item.liters).toFixed(1)} L
            </div>
          </div>
          <div style="text-align:right">
            <div class="score">Nota ${item.score}</div>
            <div style="color:var(--muted);font-size:11px">R$ ${Number(item.cost).toFixed(2).replace('.',',')}</div>
          </div>
        </div>
      `;
    });
    list.innerHTML = html;
    document.getElementById('histAvg').textContent = (totalL / data.itens.length).toFixed(1) + ' L';
  }catch(e){}
}

function parseTariffInput(){
  let raw = document.getElementById('cfgTariff').value.replace(',', '.').replace(/[^0-9.]/g, '');
  let val = parseFloat(raw);
  return isNaN(val) ? 12.50 : val;
}

function updateTariffHelper(){
  let val = parseTariffInput();
  let costPerLiter = val / 1000.0;
  let cost30L = costPerLiter * 30.0;
  document.getElementById('tariffHelper').innerHTML = 
    `💰 Equivale a <strong>R$ ${costPerLiter.toFixed(4).replace('.',',')}</strong> por litro (ou <strong>~R$ ${cost30L.toFixed(2).replace('.',',')}</strong> por banho de 30L).`;
}

async function fetchConfig(){
  try{
    const cfg = await api('/api/config');
    let t = Number(cfg.tarifaM3 || 12.50).toFixed(2).replace('.', ',');
    let p = Number(cfg.pulsosL || 450).toFixed(1);
    document.getElementById('cfgTariff').value = t;
    document.getElementById('curPulsesDisplay').textContent = p + ' pulsos/L';
    document.getElementById('cfgManualPulses').value = Math.round(Number(cfg.pulsosL || 450));
    updateTariffHelper();
    let comp = getActiveCompany();
    let nameEl = document.getElementById('cfgActiveCompName');
    if(nameEl && comp) nameEl.textContent = comp.name;
  }catch(e){}
}

async function saveTariff(){
  let val = parseTariffInput();
  await api(`/api/config?tariff=${val}`, {method:'POST'});
  const msg = document.getElementById('tariffMsg');
  msg.style.display = 'block';
  setTimeout(()=>msg.style.display='none', 3000);
}

// CALIBRAÇÃO
function setCalibVol(ml){
  document.getElementById('inpCalibVol').value = ml;
  updateCalibChipSelection();
}

function stepCalibVol(delta){
  const inp = document.getElementById('inpCalibVol');
  let val = Math.max(100, Math.min(10000, (parseInt(inp.value)||500) + delta));
  inp.value = val;
  updateCalibChipSelection();
}

function updateCalibChipSelection(){
  let val = parseInt(document.getElementById('inpCalibVol').value)||500;
  document.querySelectorAll('.calib-box .chip').forEach(c => {
    c.classList.toggle('active', parseInt(c.textContent) === val);
  });
}

async function startCalibration(){
  try{
    document.getElementById('calibResultCard').classList.remove('show');
    await api('/api/calib/start', {method:'POST'});
    isCalibrating = true;
    document.getElementById('btnStartCalib').style.display = 'none';
    document.getElementById('btnStopCalib').style.display = 'flex';
    document.getElementById('calibActiveArea').style.display = 'block';
    document.getElementById('calibPulsesCount').textContent = '0';

    if(calibTimer) clearInterval(calibTimer);
    calibTimer = setInterval(async ()=>{
      try{
        const st = await api('/api/calib');
        document.getElementById('calibPulsesCount').textContent = st.pulses || 0;
      }catch(e){}
    }, 500);
  }catch(e){
    alert('Erro ao iniciar calibração');
  }
}

async function stopCalibration(){
  try{
    if(calibTimer) clearInterval(calibTimer);
    const res = await api('/api/calib/stop', {method:'POST'});
    isCalibrating = false;
    document.getElementById('btnStartCalib').style.display = 'flex';
    document.getElementById('btnStopCalib').style.display = 'none';
    document.getElementById('calibActiveArea').style.display = 'none';

    let pulses = parseInt(res.pulses)||0;
    let ml = parseInt(document.getElementById('inpCalibVol').value)||500;
    let liters = ml / 1000.0;

    if(pulses <= 0){
      alert('Nenhum pulso detectado. Verifique o fluxo de água no sensor.');
      return;
    }

    calculatedFactor = pulses / liters;
    let diffPct = ((calculatedFactor - 450.0) / 450.0) * 100.0;
    let diffStr = (diffPct >= 0 ? '+' : '') + diffPct.toFixed(1) + '%';

    document.getElementById('calibResultDetails').innerHTML = `
      Foram contados <strong>${pulses} pulsos</strong> para <strong>${ml} ml</strong> (${liters.toFixed(2)} L).<br>
      Fator calculado: <strong style="color:var(--accent);font-size:16px">${calculatedFactor.toFixed(1)} pulsos/Litro</strong><br>
      <span style="color:var(--muted);font-size:11px">Variação de ${diffStr} em relação ao padrão de 450.0</span>
    `;
    document.getElementById('calibResultCard').classList.add('show');
  }catch(e){
    alert('Erro ao concluir calibração');
  }
}

async function applyCalculatedCalib(){
  await api(`/api/config?pulses=${calculatedFactor.toFixed(1)}`, {method:'POST'});
  document.getElementById('curPulsesDisplay').textContent = calculatedFactor.toFixed(1) + ' pulsos/L';
  document.getElementById('cfgManualPulses').value = Math.round(calculatedFactor);
  document.getElementById('calibResultCard').classList.remove('show');
  showCalibSuccess('Calibração aplicada com sucesso!');
}

async function saveManualPulses(){
  let p = parseFloat(document.getElementById('cfgManualPulses').value);
  if(isNaN(p) || p < 10) return;
  await api(`/api/config?pulses=${p}`, {method:'POST'});
  document.getElementById('curPulsesDisplay').textContent = p.toFixed(1) + ' pulsos/L';
  showCalibSuccess('Calibração manual aplicada!');
}

async function resetFactoryCalib(){
  await api('/api/calib/reset', {method:'POST'});
  document.getElementById('curPulsesDisplay').textContent = '450.0 pulsos/L';
  document.getElementById('cfgManualPulses').value = 450;
  document.getElementById('calibResultCard').classList.remove('show');
  showCalibSuccess('Padrão de fábrica restaurado (450.0 pulsos/L)!');
}

function showCalibSuccess(text){
  const msg = document.getElementById('calibSuccessMsg');
  msg.textContent = text;
  msg.style.display = 'block';
  setTimeout(()=>msg.style.display='none', 3000);
}

async function poll(){
  try{
    const s = await api('/api/status');
    const badge = document.getElementById('statusBadge');
    badge.textContent = '● Conectado';
    badge.classList.remove('offline');

    // Litros, Vazão, Tempo e Custo
    document.getElementById('liveLiters').textContent = Number(s.liters).toFixed(1);
    document.getElementById('liveFlow').innerHTML = Number(s.flow).toFixed(1) + ' <span style="font-size:11px;font-weight:normal">L/m</span>';
    document.getElementById('liveTime').textContent = formatSec(s.seconds);
    document.getElementById('liveCost').textContent = 'R$ ' + Number(s.cost).toFixed(2).replace('.',',');

    // Estados
    const pill = document.getElementById('statePill');
    isRunning = !!s.running;
    isPaused = (s.state === 'paused');

    if(s.state === 'paused'){
      pill.textContent = '🧼 Modo Ensaboar';
      pill.className = 'status-pill paused';
    } else if(isRunning){
      pill.textContent = '💧 Banho em Andamento';
      pill.className = 'status-pill active';
    } else if(s.state === 'finished'){
      pill.textContent = '🏁 Banho Finalizado';
      pill.className = 'status-pill';
    } else {
      pill.textContent = 'Pronto';
      pill.className = 'status-pill';
    }

    // Botões
    document.getElementById('btnStart').style.display = isRunning ? 'none' : 'flex';
    document.getElementById('btnPause').style.display = isRunning ? 'flex' : 'none';
    document.getElementById('btnStop').style.display = isRunning ? 'flex' : 'none';

    if(isPaused){
      document.getElementById('btnPause').textContent = '▶ RETOMAR BANHO';
    } else {
      document.getElementById('btnPause').textContent = '🧼 MODO ENSABOAR';
    }

    // Barra de progresso
    let pct = 0;
    if(s.mode === 'timer' && s.targetSeconds > 0){
      pct = Math.min(100, (s.seconds / s.targetSeconds) * 100);
    } else if(s.mode === 'liters' && s.targetLiters > 0){
      pct = Math.min(100, (Number(s.liters) / s.targetLiters) * 100);
    } else {
      pct = Math.min(100, (Number(s.liters) / 50) * 100);
    }
    document.getElementById('liveProgress').style.width = pct + '%';

    // Card de Resultado
    const resCard = document.getElementById('resultCard');
    if(s.state === 'finished'){
      resCard.classList.add('show');
      document.getElementById('resScore').textContent = 'Nota ' + s.score;
      document.getElementById('resText').textContent = 
        `${Number(s.liters).toFixed(1)} Litros em ${formatSec(s.seconds)} • Custo: R$ ${Number(s.cost).toFixed(2).replace('.',',')}`;
    } else {
      resCard.classList.remove('show');
    }

  }catch(e){
    const badge = document.getElementById('statusBadge');
    badge.textContent = '● Sem resposta';
    badge.classList.add('offline');
  }
}

// ======================================================
// CONCESSIONÁRIAS DE ÁGUA E ESTIMADOR MENSAL (EMBASA)
// ======================================================
const PRESET_COMPANIES = [
  {
    id: 'embasa_normal',
    name: 'EMBASA - Residencial Normal (Bahia)',
    desc: 'Regra EMBASA: 0 a 6 m³ fixo (R$ 44,77) + excedente progressivo por faixas + 80% esgoto.',
    minM3: 6,
    minCost: 44.77,
    sewagePct: 80,
    tiers: [
      { max: 6, rate: 0.0, label: '0 a 6 m³ (Mínimo Fixo)' },
      { max: 10, rate: 8.85, label: '7 a 10 m³' },
      { max: 15, rate: 11.20, label: '11 a 15 m³' },
      { max: 20, rate: 13.80, label: '16 a 20 m³' },
      { max: 30, rate: 16.90, label: '21 a 30 m³' },
      { max: 50, rate: 20.40, label: '31 a 50 m³' },
      { max: 9999, rate: 24.10, label: 'Acima de 50 m³' }
    ]
  },
  {
    id: 'embasa_social',
    name: 'EMBASA - Residencial Social',
    desc: 'Regra EMBASA: CadÚnico / Baixa Renda. Mínimo 0 a 6 m³ (R$ 16,28) + 80% esgoto.',
    minM3: 6,
    minCost: 16.28,
    sewagePct: 80,
    tiers: [
      { max: 6, rate: 0.0, label: '0 a 6 m³ (Mínimo Fixo)' },
      { max: 10, rate: 4.40, label: '7 a 10 m³' },
      { max: 15, rate: 7.80, label: '11 a 15 m³' },
      { max: 20, rate: 11.50, label: '16 a 20 m³' },
      { max: 9999, rate: 15.20, label: 'Acima de 20 m³' }
    ]
  },
  {
    id: 'embasa_comercial',
    name: 'EMBASA - Comercial',
    desc: 'Regra EMBASA: Comércio e Serviços. Mínimo 0 a 6 m³ (R$ 74,50) + 80% esgoto.',
    minM3: 6,
    minCost: 74.50,
    sewagePct: 80,
    tiers: [
      { max: 6, rate: 0.0, label: '0 a 6 m³ (Mínimo Fixo)' },
      { max: 10, rate: 12.30, label: '7 a 10 m³' },
      { max: 20, rate: 16.80, label: '11 a 20 m³' },
      { max: 50, rate: 21.90, label: '21 a 50 m³' },
      { max: 9999, rate: 26.50, label: 'Acima de 50 m³' }
    ]
  }
];

let activeCompanyId = 'embasa_normal';
let activeSewagePct = 80;
let simMode = 'house';
let showerWeightPct = 40;
let lastCalculatedEffectiveRate = 13.43;

function getCustomCompanies(){
  try{
    return JSON.parse(localStorage.getItem('smartshower_custom_companies') || '[]');
  }catch(e){ return []; }
}

function saveCustomCompanies(list){
  try{
    localStorage.setItem('smartshower_custom_companies', JSON.stringify(list));
  }catch(e){}
}

function getAllCompanies(){
  return PRESET_COMPANIES.concat(getCustomCompanies());
}

function getActiveCompany(){
  let all = getAllCompanies();
  return all.find(c => c.id === activeCompanyId) || all[0];
}

function initBilling(){
  activeCompanyId = localStorage.getItem('smartshower_active_company_id') || 'embasa_normal';
  let savedSewage = localStorage.getItem('smartshower_sewage_pct');
  if(savedSewage !== null) activeSewagePct = parseInt(savedSewage);
  populateCompanySelect();
  updateSewageChips();
  recalcBill();
}

function populateCompanySelect(){
  const sel = document.getElementById('selConcessionaire');
  if(!sel) return;
  sel.innerHTML = '';
  let all = getAllCompanies();
  all.forEach(c => {
    let opt = document.createElement('option');
    opt.value = c.id;
    opt.textContent = c.name;
    if(c.id === activeCompanyId) opt.selected = true;
    sel.appendChild(opt);
  });
  updateCompanyDesc();
}

function updateCompanyDesc(){
  const c = getActiveCompany();
  const descEl = document.getElementById('companyDesc');
  const badgeEl = document.getElementById('billCompBadge');
  if(descEl) descEl.textContent = c.desc || '';
  if(badgeEl) badgeEl.textContent = c.name;
}

function onCompanySelectChange(){
  activeCompanyId = document.getElementById('selConcessionaire').value;
  localStorage.setItem('smartshower_active_company_id', activeCompanyId);
  const comp = getActiveCompany();
  setSewagePct(comp.sewagePct || 80);
  updateCompanyDesc();
  recalcBill();
}

function toggleCustomCompanyForm(show){
  const box = document.getElementById('boxCustomCompany');
  if(!box) return;
  if(show === undefined) show = (box.style.display === 'none');
  box.style.display = show ? 'block' : 'none';
}

function saveCustomCompany(){
  let name = (document.getElementById('custCompName').value || '').trim();
  let minM3 = parseFloat(document.getElementById('custCompMinM3').value) || 6;
  let minCost = parseFloat(document.getElementById('custCompMinCost').value) || 40.0;
  let sewage = parseFloat(document.getElementById('custCompSewage').value) || 80;
  let tier2Rate = parseFloat(document.getElementById('custCompTier2').value) || 8.0;

  if(!name){
    alert('Por favor, informe o nome da concessionária.');
    return;
  }

  let customId = 'custom_' + Date.now();
  let newComp = {
    id: customId,
    name: name + ' (Personalizada)',
    desc: `Tarifa customizada: Mínimo ${minM3} m³ (R$ ${minCost.toFixed(2)}) + ${sewage}% esgoto.`,
    minM3: minM3,
    minCost: minCost,
    sewagePct: sewage,
    tiers: [
      { max: minM3, rate: 0.0, label: `0 a ${minM3} m³ (Mínimo Fixo)` },
      { max: minM3 + 4, rate: tier2Rate, label: `${minM3 + 1} a ${minM3 + 4} m³` },
      { max: minM3 + 9, rate: tier2Rate * 1.3, label: `${minM3 + 5} a ${minM3 + 9} m³` },
      { max: 9999, rate: tier2Rate * 1.7, label: `Acima de ${minM3 + 9} m³` }
    ]
  };

  let list = getCustomCompanies();
  list.push(newComp);
  saveCustomCompanies(list);

  activeCompanyId = customId;
  localStorage.setItem('smartshower_active_company_id', activeCompanyId);
  populateCompanySelect();
  toggleCustomCompanyForm(false);
  setSewagePct(sewage);
  recalcBill();
}

function setSewagePct(pct){
  activeSewagePct = pct;
  localStorage.setItem('smartshower_sewage_pct', pct);
  updateSewageChips();
  recalcBill();
}

function updateSewageChips(){
  document.querySelectorAll('#sewageChipGroup .chip').forEach(c => {
    let val = parseInt(c.textContent) || 0;
    c.classList.toggle('active', val === activeSewagePct);
  });
  const sewagePctText = document.getElementById('billSewagePctText');
  if(sewagePctText) sewagePctText.textContent = activeSewagePct + '%';
}

function setSimMode(mode){
  simMode = mode;
  document.getElementById('btnSimModeHouse').classList.toggle('active', mode === 'house');
  document.getElementById('btnSimModeDirect').classList.toggle('active', mode === 'direct');
  document.getElementById('boxSimHouse').style.display = (mode === 'house') ? 'block' : 'none';
  document.getElementById('boxSimDirect').style.display = (mode === 'direct') ? 'block' : 'none';
  recalcBill();
}

function stepResidents(delta){
  const inp = document.getElementById('inpResidents');
  let val = Math.max(1, Math.min(15, (parseInt(inp.value)||3) + delta));
  inp.value = val;
  recalcBill();
}

function stepDailyBaths(delta){
  const inp = document.getElementById('inpDailyBaths');
  let val = Math.max(1, Math.min(5, (parseInt(inp.value)||1) + delta));
  inp.value = val;
  recalcBill();
}

async function useSmartShowerAvg(){
  try{
    let curAvgStr = document.getElementById('histAvg').textContent;
    let avg = parseFloat(curAvgStr);
    if(isNaN(avg) || avg <= 0){
      const data = await api('/api/history');
      if(data.itens && data.itens.length > 0){
        let sum = 0;
        data.itens.forEach(i => sum += Number(i.liters));
        avg = sum / data.itens.length;
      }
    }
    if(!isNaN(avg) && avg > 0){
      document.getElementById('inpHouseAvgShower').value = Math.round(avg);
      recalcBill();
    } else {
      alert('Nenhum banho registrado no SmartShower ainda. O valor de 32L foi mantido.');
    }
  }catch(e){
    alert('Não foi possível obter a média do histórico.');
  }
}

function setShowerWeight(w){
  showerWeightPct = w;
  document.querySelectorAll('#showerWeightGroup .chip').forEach(c => {
    c.classList.toggle('active', parseInt(c.textContent) === w);
  });
  recalcBill();
}

function setDirectM3(m3){
  document.getElementById('inpDirectM3').value = m3;
  document.querySelectorAll('#boxSimDirect .chip').forEach(c => {
    c.classList.toggle('active', parseFloat(c.textContent) === m3);
  });
  recalcBill();
}

function stepDirectM3(delta){
  const inp = document.getElementById('inpDirectM3');
  let val = Math.max(1, Math.min(500, (parseFloat(inp.value)||12) + delta));
  inp.value = val;
  document.querySelectorAll('#boxSimDirect .chip').forEach(c => {
    c.classList.toggle('active', parseFloat(c.textContent) === val);
  });
  recalcBill();
}

function calculateBill(volM3, company, sewagePct){
  let vol = Math.max(0, parseFloat(volM3) || 0);
  let pctEsgoto = (sewagePct !== undefined) ? sewagePct : company.sewagePct;

  let waterCost = company.minCost;
  let breakdown = [];

  let volMin = company.minM3;
  breakdown.push({
    label: company.tiers[0].label,
    volInTier: volMin,
    volBilled: Math.min(vol, volMin),
    rate: (company.minCost / volMin),
    subtotal: company.minCost,
    isMin: true,
    isActive: (vol <= volMin)
  });

  let currentTierIndex = 0;
  let currentTierLabel = company.tiers[0].label;
  let tierLower = 0;
  let tierUpper = volMin;
  let nextTierThreshold = volMin;
  let nextTierRate = (company.tiers.length > 1) ? company.tiers[1].rate : 0;

  if(vol > volMin){
    for(let i = 1; i < company.tiers.length; i++){
      let prevMax = company.tiers[i - 1].max;
      let currMax = company.tiers[i].max;

      if(vol > prevMax){
        let volInTier = Math.min(vol, currMax) - prevMax;
        let subtotal = volInTier * company.tiers[i].rate;
        waterCost += subtotal;

        let isActive = (vol <= currMax || i === company.tiers.length - 1);
        if(isActive){
          currentTierIndex = i;
          currentTierLabel = company.tiers[i].label;
          tierLower = prevMax;
          tierUpper = currMax;
          if(i + 1 < company.tiers.length){
            nextTierThreshold = currMax;
            nextTierRate = company.tiers[i + 1].rate;
          } else {
            nextTierThreshold = 9999;
            nextTierRate = company.tiers[i].rate;
          }
        }

        breakdown.push({
          label: company.tiers[i].label,
          volInTier: (currMax < 9000 ? (currMax - prevMax) : 'Exced.'),
          volBilled: volInTier,
          rate: company.tiers[i].rate,
          subtotal: subtotal,
          isMin: false,
          isActive: isActive
        });
      }
    }
  }

  let sewageCost = waterCost * (pctEsgoto / 100.0);
  let totalCost = waterCost + sewageCost;
  let effectiveRate = (vol > 0) ? (totalCost / vol) : (totalCost / volMin);

  return {
    vol: vol,
    waterCost: waterCost,
    sewageCost: sewageCost,
    totalCost: totalCost,
    effectiveRate: effectiveRate,
    breakdown: breakdown,
    currentTierIndex: currentTierIndex,
    currentTierLabel: currentTierLabel,
    tierLower: tierLower,
    tierUpper: tierUpper,
    nextTierThreshold: nextTierThreshold,
    nextTierRate: nextTierRate,
    remainingToNext: (nextTierThreshold < 9000) ? Math.max(0, nextTierThreshold - vol) : 0
  };
}

function recalcBill(){
  const comp = getActiveCompany();
  let estimatedM3 = 0;

  if(simMode === 'house'){
    let residents = parseInt(document.getElementById('inpResidents').value) || 3;
    let baths = parseInt(document.getElementById('inpDailyBaths').value) || 1;
    let avgLiters = parseFloat(document.getElementById('inpHouseAvgShower').value) || 32;

    let showerLMonth = residents * baths * 30 * avgLiters;
    let totalHouseL = showerLMonth / (showerWeightPct / 100.0);
    estimatedM3 = totalHouseL / 1000.0;

    const simHelper = document.getElementById('houseSimHelper');
    if(simHelper){
      simHelper.innerHTML = `🚿 Banhos: <strong>${Math.round(showerLMonth).toLocaleString('pt-BR')} L/mês</strong> • Casa toda: <strong>~${estimatedM3.toFixed(2).replace('.', ',')} m³ (${Math.round(totalHouseL).toLocaleString('pt-BR')} L)</strong>`;
    }
  } else {
    estimatedM3 = parseFloat(document.getElementById('inpDirectM3').value) || 12;
  }

  const bill = calculateBill(estimatedM3, comp, activeSewagePct);
  lastCalculatedEffectiveRate = bill.effectiveRate;

  // Atualiza Hero Card
  document.getElementById('billTotalAmount').textContent = 'R$ ' + bill.totalCost.toFixed(2).replace('.', ',');
  document.getElementById('billSubtitle').innerHTML = 
    `Calculado para <strong>${bill.vol.toFixed(1).replace('.', ',')} m³</strong> no mês (tarifa média R$ ${bill.effectiveRate.toFixed(2).replace('.', ',')}/m³)`;
  document.getElementById('billWaterVal').textContent = 'R$ ' + bill.waterCost.toFixed(2).replace('.', ',');
  document.getElementById('billSewageVal').textContent = 'R$ ' + bill.sewageCost.toFixed(2).replace('.', ',');

  // Atualiza Termômetro de Faixa
  document.getElementById('tierBadge').textContent = bill.currentTierLabel;
  document.getElementById('tierMeterCurrent').textContent = `Consumo: ${bill.vol.toFixed(1)} m³`;
  document.getElementById('tierMeterMin').textContent = `${bill.tierLower} m³`;
  document.getElementById('tierMeterMax').textContent = (bill.tierUpper < 9000 ? `${bill.tierUpper} m³` : 'Max');

  let pctInTier = 100;
  if(bill.tierUpper < 9000){
    let span = bill.tierUpper - bill.tierLower;
    let pos = bill.vol - bill.tierLower;
    pctInTier = Math.min(100, Math.max(0, (pos / span) * 100));
  }
  document.getElementById('tierMeterFill').style.width = pctInTier + '%';

  const alertBox = document.getElementById('tierAlertBox');
  if(bill.remainingToNext > 0){
    let remLitros = Math.round(bill.remainingToNext * 1000);
    alertBox.innerHTML = `
      ⚠️ <strong>Atenção à faixa:</strong> Faltam apenas <strong>${bill.remainingToNext.toFixed(1).replace('.', ',')} m³ (${remLitros} Litros)</strong> para entrar na próxima faixa mais cara (R$ ${bill.nextTierRate.toFixed(2).replace('.', ',')}/m³)!<br>
      💡 <em>Economizando 2 minutos de cada banho diário, sua casa evita subir de faixa tarifária.</em>
    `;
    alertBox.style.display = 'block';
  } else {
    alertBox.innerHTML = `
      💡 <strong>Faixa Superior:</strong> Sua residência já atingiu as faixas com tarifa de água de maior valor. Cada litro economizado no banho gera a máxima economia na conta!
    `;
    alertBox.style.display = 'block';
  }

  // Atualiza Tabela de Extrato
  const tbody = document.getElementById('billTableBody');
  if(tbody){
    let rowsHtml = '';
    bill.breakdown.forEach(item => {
      let activeClass = item.isActive ? ' class="active-tier"' : '';
      let rateStr = item.isMin ? 'Fixo' : ('R$ ' + item.rate.toFixed(2).replace('.', ','));
      let volStr = item.isMin ? `${item.volInTier} m³` : `${item.volBilled.toFixed(1).replace('.', ',')} m³`;
      rowsHtml += `
        <tr${activeClass}>
          <td>${item.label}</td>
          <td class="num">${volStr}</td>
          <td class="num">${rateStr}</td>
          <td class="num">R$ ${item.subtotal.toFixed(2).replace('.', ',')}</td>
        </tr>
      `;
    });

    // Linha do Esgoto
    rowsHtml += `
      <tr style="border-top:1px solid var(--border)">
        <td>🧪 Esgotamento Sanitário (${bill.pctEsgoto}%)</td>
        <td class="num">--</td>
        <td class="num">${bill.pctEsgoto}%</td>
        <td class="num">R$ ${bill.sewageCost.toFixed(2).replace('.', ',')}</td>
      </tr>
      <tr style="font-weight:900;color:var(--ok);border-top:2px solid var(--border)">
        <td>TOTAL ESTIMADO</td>
        <td class="num">${bill.vol.toFixed(1).replace('.', ',')} m³</td>
        <td class="num">--</td>
        <td class="num">R$ ${bill.totalCost.toFixed(2).replace('.', ',')}</td>
      </tr>
    `;
    tbody.innerHTML = rowsHtml;
  }
}

async function syncTariffWithArduino(){
  try{
    let rate = lastCalculatedEffectiveRate || 13.43;
    await api(`/api/config?tariff=${rate.toFixed(2)}`, {method:'POST'});
    
    // Atualiza campo na aba de ajustes
    let cfgTariffInp = document.getElementById('cfgTariff');
    if(cfgTariffInp) cfgTariffInp.value = rate.toFixed(2).replace('.', ',');
    updateTariffHelper();

    const msg = document.getElementById('billSyncMsg');
    msg.style.display = 'block';
    setTimeout(() => msg.style.display = 'none', 3500);
  }catch(e){
    alert('Erro ao sincronizar tarifa com o SmartShower.');
  }
}

setInterval(poll, 1000);
poll();
</script>
</body>
</html>
)rawliteral";
