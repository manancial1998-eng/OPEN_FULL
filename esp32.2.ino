#include <WiFi.h>

const char* ssid = "Barco_Paulo_I";
const char* password = "luis";

// ip esp1
const char* host = "192.168.4.1"; // IP padrão do ESP32 em modo AP
const int port = 80;

String ultimosDados = "";
unsigned long ultimaTransmissao = 0;

void setup() {
  Serial.begin(115200);  // Comunicação com computador
  Serial2.begin(115200); // Comunicação com primeiro ESP32
  
  //ponte entre esps
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
}

void loop() {
  // Ler dados do primeiro esp
  if (Serial2.available() > 0) {
    String dados = Serial2.readStringUntil('\n');
    dados.trim();
    
    if (dados.length() > 0) {
      ultimosDados = dados;
      Serial.println("Dados recebidos: " + dados);
      
      // confirma
      Serial2.println("OK_" + String(millis()));
    }
  }
  
  // portal seria com value
  if (ultimosDados.length() > 0 && millis() - ultimaTransmissao >= 500) {
    Serial.println(ultimosDados);
    ultimaTransmissao = millis();
  }
  
  // http para dados
  if (millis() % 10000 == 0) {
    buscarDadosHTTP();
  }
  
  delay(10);
}

void buscarDadosHTTP() {
  WiFiClient client;
  
  if (!client.connect(host, port)) {
    Serial.println("Falha ao conectar com ESP32 principal");
    return;
  }

  client.println("GET /dados HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Connection: close");
  client.println();
  
  // resposta?
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        // Cabeçalhos terminam, começa o corpo
        String dados = client.readStringUntil('\n');
        dados.trim();
        if (dados.length() > 0) {
          ultimosDados = dados;
          Serial.println("Dados HTTP: " + dados);
        }
        break;
      }
    }
  }
  
  client.stop();
}