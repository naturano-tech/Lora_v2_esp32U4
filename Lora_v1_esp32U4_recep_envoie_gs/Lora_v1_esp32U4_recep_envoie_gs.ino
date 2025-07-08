#include "Arduino.h"
#include "LoRa_E32.h"
#include <WiFi.h>
kaiza
// Remplace par le nom de ton réseau Wi-Fi et ton mot de passe
const char* ssid = "PIX-LINK-2.4G";
const char* password = "naturanowifi1";
// Configuration des broches pour ESP32 38 pins
LoRa_E32 e32ttl(&Serial2, 15, 4, 2); // RX, AUX, M0, M1

void setup() {
  Serial.begin(9600);
  delay(500);

  Serial.println();
  Serial.print("Connexion au Wi-Fi : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Attente de la connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connecté au Wi-Fi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  e32ttl.begin();
  Serial.println("Module LoRa initialisé - Prêt à envoyer des messages");

}

void loop() {
Serial.println(receiving_data());
}
String receiving_data(){
  if (e32ttl.available() > 1) {
    ResponseContainer rc = e32ttl.receiveMessage();
    if (rc.status.code != 1) {
      return rc.status.getResponseDescription();
    } else {
      return rc.data;
    }
  }
}