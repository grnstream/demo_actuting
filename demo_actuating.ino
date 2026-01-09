/*
  ThingsInNet Actuating Board
  AP-mode Web Dashboard for relay control

  Check the Dev Kit: https://thingsinnet.com/product/esp32-universal-iot-dev-kit-actuating/ 

  Actuators used:
  - Relay 1 : GPIO25
  - Relay 2 : GPIO14
  - Relay 3 : GPIO16
  - Relay 4 : GPIO17

  UI / Status:
  - SSD1306 OLED (I2C, default address 0x3C)

  Notes:
  - Many relay boards are ACTIVE LOW (LOW = ON, HIGH = OFF)
*/

#include <WiFi.h>
#include <WebServer.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// array size 1024
const unsigned char logo[] PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x20, 0x0c, 0x00, 0x87, 0xbc, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0x1e, 0x00, 0x87, 0xfd, 0x80, 0xc0, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x0c, 0x00, 0x40, 
  0x00, 0xdf, 0xd6, 0x00, 0x80, 0x41, 0x80, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x08, 0x00, 0x60, 
  0x00, 0x70, 0x1e, 0x00, 0x80, 0x41, 0xb0, 0x01, 0x81, 0x81, 0x83, 0x17, 0x0e, 0x08, 0x38, 0xf0, 
  0x00, 0x00, 0x06, 0x00, 0x80, 0x41, 0xf8, 0xcf, 0xc7, 0x72, 0x63, 0x1f, 0x8b, 0x08, 0xcc, 0x60, 
  0x00, 0xc1, 0x02, 0x00, 0x80, 0x41, 0x8c, 0xcc, 0x66, 0x36, 0x03, 0x18, 0x89, 0xc8, 0x86, 0x60, 
  0x01, 0x8f, 0x03, 0x00, 0x80, 0x41, 0x8c, 0xcc, 0x6c, 0x33, 0x83, 0x18, 0x88, 0xe9, 0x86, 0x60, 
  0x01, 0x1b, 0x01, 0x80, 0x80, 0x41, 0x8c, 0xcc, 0x6c, 0x31, 0xc3, 0x18, 0x88, 0x69, 0xfe, 0x60, 
  0x03, 0x17, 0x01, 0x80, 0x80, 0x41, 0x8c, 0xcc, 0x64, 0x30, 0x63, 0x18, 0x88, 0x38, 0x80, 0x60, 
  0x0e, 0x3d, 0x00, 0x60, 0x80, 0x41, 0x8c, 0xcc, 0x66, 0x36, 0x23, 0x18, 0x88, 0x18, 0xc4, 0x60, 
  0x1e, 0x3b, 0x80, 0xf0, 0x80, 0x41, 0x8c, 0xcc, 0x63, 0xf3, 0xe3, 0x18, 0x88, 0x08, 0x7c, 0x38, 
  0x1a, 0x3a, 0xfc, 0xd0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0e, 0x03, 0xb8, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x78, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0x01, 0xd9, 0x80, 0x80, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x80, 0xf1, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x81, 0xc3, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xc0, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x60, 0x0c, 0x00, 0x81, 0xb6, 0x60, 0x92, 0x0a, 0xcd, 0x89, 0x61, 0xde, 0x66, 0x02, 0x40, 
  0x00, 0x70, 0x1e, 0x00, 0x81, 0x5d, 0xde, 0xd7, 0x19, 0xbf, 0xfe, 0x61, 0x9f, 0xdb, 0xff, 0xc0, 
  0x00, 0xd7, 0xf6, 0x00, 0x81, 0xdd, 0xe5, 0xd6, 0x0f, 0xd3, 0x7b, 0x21, 0x95, 0xfe, 0xf6, 0x80, 
  0x00, 0x70, 0x1c, 0x00, 0x80, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 
  0x00, 0x20, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


unsigned char fBuffer[1024] = {0};
  

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// ====== Wi-Fi AP Settings ======
const char *ap_ssid     = "TIN-Actuating";
const char *ap_password = "12345678"; // minimum 8 chars

// Input Push Buttons
const int pushBtn[4] = {34, 35, 39, 36};
bool btnState[4] = { false, false, false, false };  // Active High
const bool BTN_ACTIVE_HIGH = true;

// Buzzer & On board LED
const int Buzzer = 26;
const int LED2 = 2;

// ====== Relay Pin Settings ======
// Change these pins to match your ThingsInNet actuating board
//const int relayPins[4] = { 26, 27, 14, 12 };  // for Sensing
const int relayPins[4] = { 25, 14, 16, 17 };    // for Actuating

// Many relay modules are ACTIVE LOW:
//  - LOW  = Relay ON
//  - HIGH = Relay OFF
// If your board is ACTIVE HIGH, set this to true.
const bool RELAY_ACTIVE_HIGH = false;

// Relay states: true = ON, false = OFF (logical state)
bool relayState[4] = { false, false, false, false };

unsigned long lastPingTime = 0;
IPAddress lastClientIP;
bool clientConnected = false;

WebServer server(80);

// ====== HTML UI (served from flash) ======
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ThingsInNet – Relay Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    :root {
  color-scheme: dark;
  }
  * {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    font-family: system-ui, -apple-system, BlinkMacSystemFont, "SF Pro Text",
               "Segoe UI", sans-serif;
  }
    body {
      margin: 0;
      background: radial-gradient(circle at top, #1e293b 0, #020617 45%, #000 100%);
      color: #e5e7eb;
      min-height: 100vh;
    }
    .overlay {
      position: fixed;
      inset: 0;
      display: flex;
      align-items: center;
      justify-content: center;
      flex-direction: column;
      background: radial-gradient(circle at top, #111827, #020617);
      z-index: 10;
    }
    .overlay h1 {
      margin: 0;
      font-size: 2.4rem;
      letter-spacing: 0.1em;
      text-transform: uppercase;
    }
    .overlay p {
      margin-top: 0.6rem;
      font-size: 0.95rem;
      opacity: 0.7;
    }
    .spinner {
      margin-top: 1.5rem;
      width: 36px;
      height: 36px;
      border-radius: 999px;
      border: 3px solid #1f2937;
      border-top-color: #22c55e;
      animation: spin 0.8s linear infinite;
    }
    @keyframes spin {
      to { transform: rotate(360deg); }
    }

    .hidden {
      display: none;
    }

    .container {
      max-width: 960px;
      margin: 0 auto;
      padding: 1.25rem;
    }
    header {
      padding-top: 4.5rem;
      padding-bottom: 1rem;
      text-align: center;
    }
    header h2 {
      margin: 0;
      font-size: 1.6rem;
      letter-spacing: 0.06em;
      text-transform: uppercase;
    }
    header p {
      margin-top: 0.5rem;
      font-size: 0.9rem;
      opacity: 0.75;
    }
    .badge {
      display: inline-block;
      font-size: 0.8rem;
      padding: 0.15rem 0.55rem;
      border-radius: 999px;
      border: 1px solid #22c55e44;
      background: #16a34a11;
      color: #bbf7d0;
      margin-top: 0.5rem;
    }

    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(190px, 1fr));
      gap: 1rem;
      margin-top: 1.25rem;
      padding-bottom: 2.5rem;
    }
    .card {
      background: linear-gradient(145deg, #020617, #030712, #020617);
      border-radius: 1rem;
      padding: 1.1rem 1.1rem 1.2rem;
      border: 1px solid #1f2937;
      box-shadow: 0 18px 45px rgba(0, 0, 0, 0.7);
    }
    .card h3 {
      margin: 0;
      font-size: 1rem;
      letter-spacing: 0.05em;
      text-transform: uppercase;
    }
    .status-line {
      margin-top: 0.45rem;
      font-size: 0.9rem;
    }
    .status {
      font-weight: 600;
    }
    .status.on {
      color: #22c55e;
    }
    .status.off {
      color: #f97373;
    }

    .toggle-btn {
      margin-top: 0.8rem;
      width: 100%;
      border-radius: 999px;
      border: none;
      padding: 0.6rem 0.8rem;
      font-size: 0.9rem;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.12s ease, box-shadow 0.12s ease, background 0.12s ease;
    }
    .toggle-btn.on {
      background: linear-gradient(135deg, #16a34a, #22c55e);
      color: #022c22;
      box-shadow: 0 8px 20px rgba(22, 163, 74, 0.45);
    }
    .toggle-btn.off {
      background: linear-gradient(135deg, #b91c1c, #ef4444);
      color: #fef2f2;
      box-shadow: 0 8px 20px rgba(239, 68, 68, 0.45);
    }
    .toggle-btn:active {
      transform: translateY(1px) scale(0.99);
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.6);
    }

    .footer-note {
      margin-top: 1.5rem;
      font-size: 0.75rem;
      text-align: center;
      opacity: 0.55;
    }
    .footer-note code {
      font-family: "JetBrains Mono", ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
      font-size: 0.7rem;
    }
  </style>
</head>
<body>
  <!-- Loading Screen -->
  <div id="loading" class="overlay">
    <h1>ThingsInNet</h1>
    <p>Preparing your actuating dashboard…</p>
    <div class="spinner"></div>
  </div>

  <!-- Main UI -->
  <div id="main" class="hidden">
    <div class="container">
      <header>
        <h2>Actuating Board – Relay Control</h2>
        <p>
          Connect to Wi-Fi: <strong>ThingsInNet-Actuator</strong><br>
          Control all 4 relays directly from this page.
        </p>
        <div class="badge">AP Mode · Local Web UI</div>
      </header>

      <div class="grid">
        <div class="card">
          <h3>Relay 1</h3>
          <div class="status-line">
            Status:
            <span id="status1" class="status off">OFF</span>
          </div>
          <button id="btn1" class="toggle-btn off" onclick="toggleRelay(1)">Turn ON</button>
        </div>

        <div class="card">
          <h3>Relay 2</h3>
          <div class="status-line">
            Status:
            <span id="status2" class="status off">OFF</span>
          </div>
          <button id="btn2" class="toggle-btn off" onclick="toggleRelay(2)">Turn ON</button>
        </div>

        <div class="card">
          <h3>Relay 3</h3>
          <div class="status-line">
            Status:
            <span id="status3" class="status off">OFF</span>
          </div>
          <button id="btn3" class="toggle-btn off" onclick="toggleRelay(3)">Turn ON</button>
        </div>

        <div class="card">
          <h3>Relay 4</h3>
          <div class="status-line">
            Status:
            <span id="status4" class="status off">OFF</span>
          </div>
          <button id="btn4" class="toggle-btn off" onclick="toggleRelay(4)">Turn ON</button>
        </div>
      </div>

      <div class="footer-note">
        Powered by <code>ESP32 · ThingsInNet Actuating Board</code>
      </div>
    </div>
  </div>

  <script>
    // Hide loading & show main after a short delay
    function showMain() {
      document.getElementById("loading").classList.add("hidden");
      document.getElementById("main").classList.remove("hidden");
    }
    setTimeout(showMain, 1200);

    function applyState(data) {
      for (let i = 1; i <= 4; i++) {
        const key = "r" + i;
        const isOn = data[key] === 1;
        const statusEl = document.getElementById("status" + i);
        const btnEl = document.getElementById("btn" + i);

        if (!statusEl || !btnEl) continue;

        if (isOn) {
          statusEl.textContent = "ON";
          statusEl.classList.remove("off");
          statusEl.classList.add("on");

          btnEl.classList.remove("off");
          btnEl.classList.add("on");
          btnEl.textContent = "Turn OFF";
        } else {
          statusEl.textContent = "OFF";
          statusEl.classList.remove("on");
          statusEl.classList.add("off");

          btnEl.classList.remove("on");
          btnEl.classList.add("off");
          btnEl.textContent = "Turn ON";
        }
      }
    }

    function fetchState() {
      fetch("/state")
        .then(function (res) { return res.json(); })
        .then(function (data) { applyState(data); })
        .catch(function (err) {
          console.log("State error:", err);
        });
    }

    function toggleRelay(n) {
      fetch("/toggle?relay=" + n)
        .then(function (res) { return res.json(); })
        .then(function (data) { applyState(data); })
        .catch(function (err) {
          console.log("Toggle error:", err);
        });
    }

    function sendPing() {
      fetch('/alive');
    }
    // Send a ping every 2 seconds
    setInterval(sendPing, 2000);
    sendPing();

    // Initial load and periodic refresh
    window.addEventListener("load", function () {
      fetchState();
      setInterval(fetchState, 1500);
    });
  </script>
</body>
</html>
)rawliteral";

// ====== Push Button Handler ======
void handlePushBtn() {
  bool buzzerState = false;
  for (int i = 0; i < 4; i++) {
    bool state = !!digitalRead(pushBtn[i]);
    
    if(BTN_ACTIVE_HIGH) btnState[i] = state ? HIGH : LOW;
    else btnState[i] = state ? LOW : HIGH;

    buzzerState |= btnState[i];
  }

  digitalWrite(Buzzer, buzzerState);  // To activate Buzzer
  digitalWrite(LED2, buzzerState);
}

// ====== Helper: write physical relay outputs ======
void updateRelayOutputs() {
  for (int i = 0; i < 4; i++) {
    bool logicalOn = relayState[i];

    if (RELAY_ACTIVE_HIGH) {
      digitalWrite(relayPins[i], logicalOn ? HIGH : LOW);
    } else {
      // ACTIVE LOW
      digitalWrite(relayPins[i], logicalOn ? LOW : HIGH);
    }
  }
}

// ====== HTTP Handlers ======
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleState() {
  String json = "{";
  for (int i = 0; i < 4; i++) {
    json += "\"r" + String(i + 1) + "\":";
    json += (relayState[i] ? "1" : "0");
    if (i < 3) json += ",";
  }
  json += "}";
  server.send(200, "application/json", json);
}

void handleToggle() {
  if (!server.hasArg("relay")) {
    server.send(400, "text/plain", "Missing relay parameter");
    return;
  }

  int relayIndex = server.arg("relay").toInt() - 1;
  if (relayIndex < 0 || relayIndex > 3) {
    server.send(400, "text/plain", "Invalid relay index");
    return;
  }

  // Toggle logical state
  relayState[relayIndex] = !relayState[relayIndex];
  updateRelayOutputs();

  // Return updated state
  handleState();
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void handleAlive() {
  lastPingTime = millis();
  lastClientIP = server.client().remoteIP();

  if (!clientConnected) {
    clientConnected = true;
    Serial.printf("Client connected: %s\n", lastClientIP.toString().c_str());
  }

  server.send(200, "text/plain", "OK");
}

void displayLogo(bool k) {

  int i,j;

  display.clearDisplay();
  
    for (i=0; i<16; i++) {
      for (j=0; j<64; j++) {
        if(k==true) fBuffer[i+j*16] = logo[i+j*16];
        else fBuffer[i + j * 16] = 0;
      }
      display.clearDisplay();
      display.drawBitmap(0, 0, fBuffer, 128, 64, WHITE);
      display.display();
      delay(40);
    }
}

void displayInfo() {
  display.clearDisplay();  // Clear the display

  display.setTextColor(WHITE);  // Set text color to white
  display.setTextSize(1);       // Set text size to 1

  // Fixed horizontal position for labels (text before the colon)
  int16_t labelX = 0;   // Start from the left edge
  int16_t valueX = 35;  // Fixed position for values after the colon

  display.setCursor(labelX, 0);  
  display.print("Connect to network");      

  display.setCursor(labelX, 32);                    
  display.print("SSID");                       
  display.setCursor(valueX, 32);                     
  display.print(": ");
  display.print(ap_ssid); 

  
  display.setCursor(labelX, 44);  
  display.print("Pwd");    
  display.setCursor(valueX, 44);                    
  display.print(": ");
  display.print(ap_password); 
  display.setCursor(labelX, 56);  
  display.print("IP");   
  display.setCursor(valueX, 56);           
  display.print(": 192.168.4.1");           

  // Update the display
  display.display();
}

void displayStatus() {
  display.clearDisplay();  // Clear the display

  display.setTextColor(WHITE);  // Set text color to white
  display.setTextSize(1);       // Set text size to 1

  // Fixed horizontal position for labels (text before the colon)
  int16_t labelX = 0;   // Start from the left edge
  int16_t valueX = 56;  // Fixed position for values after the colon

  String titleStr = "RELAY STATUS";

  int16_t titleWidth = titleStr.length() * 6;                 // 6 characters * 6 pixels per character (size 1)
  int16_t titlePosX = (display.width() - titleWidth) / 2;     // Center horizontally
  display.setCursor(titlePosX, 0);                            // Set cursor to calculated position
  display.print(titleStr);       

  display.setCursor(labelX, 20);                    
  display.print("Relay 1");                        
  display.setCursor(valueX, 20);                     
  display.print(relayState[0] ? ": ON" : ": OFF"); 

  display.setCursor(labelX, 32);                     
  display.print("Relay 2");                       
  display.setCursor(valueX, 32);                     
  display.print(relayState[1] ? ": ON" : ": OFF"); 

  display.setCursor(labelX, 44);  
  display.print("Relay 3");    
  display.setCursor(valueX, 44);                     
  display.print(relayState[2] ? ": ON" : ": OFF"); 

  display.setCursor(labelX, 56);  
  display.print("Relay 4");    
  display.setCursor(valueX, 56);            
  display.print(relayState[3] ? ": ON" : ": OFF");   

  display.display();
}

// ====== Setup & Loop ======
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("ThingsInNet Actuating Board – AP Relay Controller");

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }
  displayLogo(true);
  delay(1000);

  // Initailze Push Buttons.
  for (int i = 0; i < 4; i++) {
    pinMode(pushBtn[i], INPUT);
  }

  // Initialize Buzzer & On board LED
  pinMode(LED2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, false);

  // Set up relay pins
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    relayState[i] = false; // start OFF
  }
  updateRelayOutputs();

  // Start Wi-Fi in AP mode
  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(ap_ssid, ap_password);

  if (apStarted) {
    Serial.print("Access Point started. SSID: ");
    Serial.println(ap_ssid);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Access Point!");
  }

  // Set up HTTP routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/state", HTTP_GET, handleState);
  server.on("/toggle", HTTP_GET, handleToggle);
  server.on("/alive", handleAlive);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started on port 80");

  displayLogo(false);

  displayInfo();
}

void loop() {
  server.handleClient();

  // Detect disconnect after 5 seconds without a ping
  if (clientConnected && millis() - lastPingTime > 5000) {
    Serial.printf("Client disconnected: %s\n", lastClientIP.toString().c_str());
    clientConnected = false; 
  }

  if (clientConnected) displayStatus();
  else displayInfo();

  handlePushBtn();

}

