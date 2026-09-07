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
  padding:10px 4px;font-size:13px;font-weight:700;cursor:pointer;transition:all .2s;
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
}
.mode-btn.active{background:var(--accent-glow);color:var(--accent);border-color:var(--accent)}
.input-row{display:flex;align-items:center;gap:10px;margin-bottom:14px}
.input-row label{font-size:13px;color:var(--muted);min-width:60px}
select,input{
  flex:1;height:44px;background:var(--card-inner);border:1px solid var(--border);
  border-radius:10px;color:var(--text);padding:0 12px;font-size:14px;outline:none;
}
select:focus,input:focus{border-color:var(--accent)}
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
</style>
</head>
<body>
<div class="app-container">
  <header>
    <div class="brand">🚿 SmartShower <span>2.0</span></div>
    <div id="statusBadge" class="badge">● Conectando</div>
  </header>

  <nav class="nav-tabs">
    <button class="nav-btn active" onclick="switchTab('tabLive')">🚿 Banho</button>
    <button class="nav-btn" onclick="switchTab('tabHistory')">📊 Histórico</button>
    <button class="nav-btn" onclick="switchTab('tabConfig')">⚙️ Ajustes</button>
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

      <div id="rowTimer" class="input-row">
        <label>Tempo:</label>
        <select id="selMinutes">
          <option value="3">3 minutos (Ultra rápido)</option>
          <option value="5" selected>5 minutos (Econômico)</option>
          <option value="8">8 minutos (Padrão)</option>
          <option value="10">10 minutos</option>
          <option value="15">15 minutos</option>
        </select>
      </div>

      <div id="rowLiters" class="input-row" style="display:none">
        <label>Meta:</label>
        <select id="selLiters">
          <option value="20">20 Litros</option>
          <option value="30" selected>30 Litros (Recomendado)</option>
          <option value="40">40 Litros</option>
          <option value="50">50 Litros</option>
          <option value="70">70 Litros</option>
        </select>
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

  <!-- ABA 3: AJUSTES -->
  <section id="tabConfig" class="tab-content">
    <div class="card">
      <div class="section-title">Parâmetros de Custo e Sensor</div>
      
      <div class="input-row">
        <label>Tarifa de Água:</label>
        <input type="number" id="cfgTariff" step="0.5" min="1" max="50" value="12.50">
        <span style="font-size:13px;color:var(--muted)">R$/m³</span>
      </div>

      <div class="input-row">
        <label>Calibração:</label>
        <input type="number" id="cfgPulses" step="1" min="100" max="2000" value="450">
        <span style="font-size:13px;color:var(--muted)">pulsos/L</span>
      </div>

      <button class="action btn-primary" style="width:100%;margin-top:10px" onclick="saveConfig()">Salvar Configurações</button>
      <div id="cfgMsg" style="font-size:12px;text-align:center;margin-top:8px;color:var(--ok);display:none">Salvo com sucesso!</div>
    </div>

    <div class="card">
      <div class="section-title">Sobre o SmartShower 2.0</div>
      <p style="font-size:13px;color:var(--muted);line-height:1.5">
        Sistema conectado para monitoramento de recursos hídricos. Desenvolvido para Arduino UNO R4 WiFi.
      </p>
    </div>
  </section>
</div>

<script>
let currentMode = 'timer';
let isRunning = false;
let isPaused = false;
let lastFinishedId = 0;
let audioCtx = null;

function switchTab(tabId){
  document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(b=>b.classList.remove('active'));
  document.getElementById(tabId).classList.add('active');
  event.target.classList.add('active');
  if(tabId==='tabHistory') fetchHistory();
  if(tabId==='tabConfig') fetchConfig();
}

function selectMode(mode){
  currentMode = mode;
  document.getElementById('btnModeTimer').classList.toggle('active', mode==='timer');
  document.getElementById('btnModeLiters').classList.toggle('active', mode==='liters');
  document.getElementById('btnModeFree').classList.toggle('active', mode==='free');

  document.getElementById('rowTimer').style.display = (mode==='timer') ? 'flex' : 'none';
  document.getElementById('rowLiters').style.display = (mode==='liters') ? 'flex' : 'none';
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
  if(currentMode === 'timer') target = document.getElementById('selMinutes').value;
  if(currentMode === 'liters') target = document.getElementById('selLiters').value;

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
            <div style="color:var(--muted);font-size:11px">R$ ${Number(item.cost).toFixed(2)}</div>
          </div>
        </div>
      `;
    });
    list.innerHTML = html;
    document.getElementById('histAvg').textContent = (totalL / data.itens.length).toFixed(1) + ' L';
  }catch(e){}
}

async function fetchConfig(){
  try{
    const cfg = await api('/api/config');
    document.getElementById('cfgTariff').value = cfg.tarifaM3 || 12.50;
    document.getElementById('cfgPulses').value = cfg.pulsosL || 450;
  }catch(e){}
}

async function saveConfig(){
  const tariff = document.getElementById('cfgTariff').value;
  const pulses = document.getElementById('cfgPulses').value;
  await api(`/api/config?tariff=${tariff}&pulses=${pulses}`, {method:'POST'});
  const msg = document.getElementById('cfgMsg');
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
    document.getElementById('liveCost').textContent = 'R$ ' + Number(s.cost).toFixed(2);

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
        `${Number(s.liters).toFixed(1)} Litros em ${formatSec(s.seconds)} • Custo: R$ ${Number(s.cost).toFixed(2)}`;
    } else {
      resCard.classList.remove('show');
    }

  }catch(e){
    const badge = document.getElementById('statusBadge');
    badge.textContent = '● Sem resposta';
    badge.classList.add('offline');
  }
}

setInterval(poll, 1000);
poll();
</script>
</body>
</html>
)rawliteral";
