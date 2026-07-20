#ifndef WEBSERIAL_H
#define WEBSERIAL_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <cstdarg>

class WebSerialClass {
public:
  void begin(const char* ssid, const char* password = nullptr) {
    WiFi.softAP(ssid, password);
    delay(100);

    _dnsServer.start(53, "*", WiFi.softAPIP());

    _server.on("/", std::bind(&WebSerialClass::_handleRoot, this));
    _server.on("/stream", std::bind(&WebSerialClass::_handleStream, this));
    _server.on("/send", HTTP_POST, [this]() {
      if (_server.hasArg("plain")) {
        _rxBuffer = _server.arg("plain");
        _server.send(200, "text/plain", "OK");
      } else {
        _server.send(400, "text/plain", "Bad Request");
      }
    });
    _server.onNotFound([this]() {
      _server.sendHeader("Location", "/", true);
      _server.send(302, "text/plain", "");
    });

    _server.begin();

    _log = "";
    println("WebSerial started");
    println("Connect to WiFi '" + String(ssid) + "' and open any browser.");
  }

  void loop() {
    _dnsServer.processNextRequest();
    _server.handleClient();
  }

  void print(const String& msg) {
    _log += msg;
    _trimLog();
  }
  void print(const char* msg) {
    print(String(msg));
  }
  void print(char c) {
    print(String(c));
  }
  void print(int n) {
    print(String(n));
  }
  void print(unsigned int n) {
    print(String(n));
  }
  void print(long n) {
    print(String(n));
  }
  void print(unsigned long n) {
    print(String(n));
  }
  void print(float n, int digits = 2) {
    print(String(n, digits));
  }
  void print(double n, int digits = 2) {
    print(String(n, digits));
  }

  void println(const String& msg) {
    print(msg);
    print('\n');
  }
  void println(const char* msg) {
    println(String(msg));
  }
  void println(char c) {
    print(c);
    print('\n');
  }
  void println(int n) {
    print(n);
    print('\n');
  }
  void println(unsigned int n) {
    print(n);
    print('\n');
  }
  void println(long n) {
    print(n);
    print('\n');
  }
  void println(unsigned long n) {
    print(n);
    print('\n');
  }
  void println(float n, int digits = 2) {
    print(n, digits);
    print('\n');
  }
  void println(double n, int digits = 2) {
    print(n, digits);
    print('\n');
  }
  void println() {
    print('\n');
  }

  void printf(const char* format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    print(String(buf));
  }

  bool available() {
    return _rxBuffer.length() > 0;
  }
  String readString() {
    String ret = _rxBuffer;
    _rxBuffer = "";
    return ret;
  }
  String readStringUntil(char terminator) {
    int idx = _rxBuffer.indexOf(terminator);
    if (idx != -1) {
      String ret = _rxBuffer.substring(0, idx);
      _rxBuffer = _rxBuffer.substring(idx + 1);
      return ret;
    }
    return "";
  }

private:
  WebServer _server{80};
  DNSServer _dnsServer;
  String _log;
  String _rxBuffer;
  static constexpr size_t MAX_LOG_SIZE = 8000;

  void _trimLog() {
    if (_log.length() > MAX_LOG_SIZE) {
      int idx = _log.indexOf('\n');
      if (idx != -1) _log = _log.substring(idx + 1);
    }
  }

  void _handleRoot() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Rocket Telemetry</title>
  <style>
    body { background: #0a0a0a; color: #00ffaa; font-family: 'Courier New', monospace; padding: 20px; margin: 0; }
    h1 { color: #ffffff; text-align: center; border-bottom: 2px solid #00ffaa; padding-bottom: 10px; }
    .console { background: #111; border: 1px solid #00ffaa; padding: 15px; border-radius: 5px; min-height: 60vh; max-height: 60vh; overflow-y: auto; font-size: 14px; line-height: 1.6; white-space: pre-wrap; }
    .input-area { margin-top: 15px; display: flex; gap: 10px; }
    .input-area input { flex: 1; padding: 12px; background: #222; color: #0f0; border: 1px solid #00ffaa; border-radius: 5px; font-family: inherit; font-size: 16px; }
    .input-area button { padding: 12px 30px; background: #00ffaa; color: #000; border: none; border-radius: 5px; font-weight: bold; font-size: 16px; cursor: pointer; }
    .input-area button:hover { background: #00cc88; }
  </style>
</head>
<body>
  <h1>🚀 Live Telemetry & Messenger</h1>
  <div class="console" id="log">Loading...</div>
  <div class="input-area">
    <input type="text" id="msgInput" placeholder="Type message and press Enter...">
    <button onclick="sendMsg()">Send</button>
  </div>
  <script>
    function updateLog() {
      fetch('/stream')
        .then(response => response.text())
        .then(data => {
          document.getElementById('log').textContent = data;
          var elem = document.getElementById('log');
          elem.scrollTop = elem.scrollHeight;
        })
        .catch(err => console.log('Fetch error:', err));
    }
    function sendMsg() {
      var input = document.getElementById('msgInput');
      var msg = input.value.trim();
      if (msg.length > 0) {
        fetch('/send', { method: 'POST', body: msg })
          .then(() => { input.value = ''; })
          .catch(err => console.log('Send error:', err));
      }
    }
    document.getElementById('msgInput').addEventListener('keypress', function(e) {
      if (e.key === 'Enter') sendMsg();
    });
    setInterval(updateLog, 1000);
    updateLog();
  </script>
</body>
</html>
)rawliteral";
    _server.send(200, "text/html", html);
  }

  void _handleStream() {
    _server.send(200, "text/plain", _log);
  }
};

extern WebSerialClass WebSerial;

#endif