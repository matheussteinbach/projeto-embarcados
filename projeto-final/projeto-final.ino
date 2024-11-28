#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>

const int sensorPin = 15;
const int ledPin = 4;

const char* ssid = "Matheus";
const char* password = "theus123";

//Your Domain name with URL path or IP address with path
String serverName = "https://aphid-renewed-centrally.ngrok-free.app";

unsigned long lastTime = 0;

// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

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
  //Send an HTTP POST request every 5 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "/controle";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
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
      // Free resources
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
