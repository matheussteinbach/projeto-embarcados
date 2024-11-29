#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

const int sensorPin = 34;
const int ledPin = 16;

const char* ssid = "Matheus";
const char* password = "theus123";

String serverName = "https://aphid-renewed-centrally.ngrok-free.app";

unsigned long lastTime = 0;

unsigned long timerDelay = 2000;

int valor = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial); 

  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado ao WiFi com IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Requisicao a cada 2 segundos
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      WiFiClient client;

      String serverPathGet = serverName + "/controle";
      String serverPathPost = serverName + "/logging";

      http.begin(serverPathGet.c_str());
      
      // GET REQUEST
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        valor = payload.toInt();
        Serial.println(valor);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Liberar recursos / Desconectar
      http.end();

      DynamicJsonDocument doc(2048);
      doc["valor_lido"] = valLuz;

      String json;
      serializeJson(doc, json);

      http.begin(client, serverPathPost.c_str());
      http.POST(json);

      Serial.print(http.getString());

      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
      //Serial.print("HTTP Response code: ");
      //Serial.println(httpResponseCode);

      http.end();
    }
    else {
      Serial.println("WiFi Desconectado");
    }
    lastTime = millis();
  }
  
  
  int valLuz = analogRead(sensorPin);

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
