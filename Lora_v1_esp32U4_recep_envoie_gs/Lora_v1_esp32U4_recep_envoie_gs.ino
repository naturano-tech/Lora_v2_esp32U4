#include "Arduino.h"
#include "LoRa_E32.h"
#include <WiFi.h>
#include <HTTPClient.h>

//LoRa_E32 e32ttl(&Serial2, 15, 4, 2); // RX, AUX, M0, M1
LoRa_E32 e32ttl(&Serial2, 15, 19, 18); // RX, AUX, M0, M1
const char* ssid = "PIX-LINK-2.4G";
const char* password = "naturanowifi1";

String googleScriptUrl = "https://script.google.com/macros/s/AKfycbxr9yEF-C5c6YXGV-L7W4TLHRVDN3qeNP8DljwKXyFsx8KxU1vjIk8MAM4ALbw8UzP2WQ/exec";

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  e32ttl.begin();

  Serial.println("Module LoRa initialisé - Connexion Wi-Fi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connecté !");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (e32ttl.available() > 1) {
    ResponseContainer rc = e32ttl.receiveMessage();
    if (rc.status.code != 1) {
      Serial.println("Erreur LoRa: " + rc.status.getResponseDescription());
    } else {
      String message = rc.data;
      message.trim();
      Serial.print("Message reçu: ");
      Serial.println(message);

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
      
        String url = googleScriptUrl + "?data=" + message; 
        Serial.println("URL envoyée: " + url);

        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          // Serial.println("Réponse Google: " + payload);
        } else {
          Serial.printf("Erreur HTTP: %d\n", httpCode);
        }
        http.end();
      } else {
        Serial.println("Wi-Fi déconnecté !");
        WiFi.reconnect(); // Tentative de reconnexion
      }
    }
  }

  if (Serial.available()) {
    String input = Serial.readString();
    e32ttl.sendMessage(input);
  }
  delay(1000); // Évite les requêtes trop rapides
}