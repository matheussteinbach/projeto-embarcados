#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>

const int sensorPin = 34;
const int ledPin = 16;

const char* ssid = "Matheus";
const char* password = "theus123";

String serverName = "https://aphid-renewed-centrally.ngrok-free.app";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

int valor = 0;
int valLuz = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao WiFi com IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      WiFiClientSecure client;
      client.setInsecure(); // Desabilitar validação SSL (para testes)

      // GET
      String serverPathGet = serverName + "/controle";
      http.begin(client, serverPathGet.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code GET: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        valor = payload.toInt();
        Serial.println(valor);
      } else {
        Serial.print("Error GET: ");
        Serial.println(httpResponseCode);
      }
      http.end();

      // POST
      String serverPathPost = serverName + "/logging";
      http.begin(client, serverPathPost.c_str());
      http.addHeader("Content-Type", "text/plain");
      String luz = String(valLuz);
      int httpResponseCodePOST = http.POST(luz);
      Serial.print("HTTP Response code POST: ");
      Serial.println(httpResponseCodePOST);
      http.end();
    } else {
      Serial.println("WiFi Desconectado");
    }
    lastTime = millis();
  }

  valLuz = analogRead(sensorPin);

  Serial.print("Luz: ");
  Serial.println(valLuz);
  Serial.print("Valor: ");
  Serial.println(valor);

  if (valLuz > valor) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
