#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi
const char* ssid = "Barco_Paulo_I";
const char* password = "alana";

// server
WebServer server(80);
WiFiClient client;

String ultimosDados = "";
unsigned long ultimoRecebimento = 0;

void setup() {
  Serial.begin(9600);   // chama Arduino
  Serial2.begin(115200); // esp2
  
  // wi-fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
  
  //server
  server.on("/", handleRoot);
  server.on("/dados", handleDados);
  server.begin();
  
  Serial.println("Servidor iniciado!");
}

void loop() {
  server.handleClient();
  
  // strings e values do ardu
  if (Serial.available() > 0) {
    String dadosArduino = Serial.readStringUntil('\n');
    dadosArduino.trim();
    
    if (dadosArduino.length() > 0) {
      ultimosDados = dadosArduino;
      ultimoRecebimento = millis();
      
      // passa pro próximo
      Serial2.println(dadosArduino);
      Serial.println("Dados enviados: " + dadosArduino);
    }
  }
  
  // conexão com esp2
  if (Serial2.available() > 0) {
    String resposta = Serial2.readStringUntil('\n');
    Serial.println("Resposta ESP2: " + resposta);
  }
}

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'>";
  html += "<title>Barco Paulo I - ESP32</title>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<style>body{font-family: Arial; background: #000; color: #0f0;}</style>";
  html += "</head><body>";
  html += "<h1>Barco Paulo I - ESP32 Principal</h1>";
  html += "<p><strong>Status:</strong> " + String((millis() - ultimoRecebimento < 3000) ? "✅ Recebendo dados" : "❌ Sem dados") + "</p>";
  html += "<p><strong>Últimos dados:</strong><br>" + ultimosDados + "</p>";
  html += "<p><strong>Tempo:</strong> " + String(millis()/1000) + "s</p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleDados() {
  server.send(200, "text/plain", ultimosDados);
}