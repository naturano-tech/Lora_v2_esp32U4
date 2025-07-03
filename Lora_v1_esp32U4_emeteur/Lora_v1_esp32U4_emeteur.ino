#include "Arduino.h"
#include "LoRa_E32.h"

// Configuration des broches pour ESP32 38 pins
LoRa_E32 e32ttl(&Serial2, 15, 4, 0); // RX, AUX, M0, M1

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialisation du module LoRa
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  e32ttl.begin();

  Serial.println("Module LoRa initialisé - Prêt à envoyer des messages");
}

void loop() {
  // Envoi périodique d'un message
  sendLora("&deviceID=lora02&batt_voltage=0.00&card_UID=e3211c05&Montant=8090&dureeDis=37");
  delay(2000);
}

void sendLora(String request_url) {
  Serial.println("Envoie via LoRa");
  const int MAX_SIZE = 58; // Taille max du message à envoyer via LoRa

  for (int i = 0; i < request_url.length(); i += MAX_SIZE) {
    String chunk = request_url.substring(i, i + MAX_SIZE);
    ResponseStatus rs;
    do {
      rs = e32ttl.sendMessage(chunk);
      delay(1000);
    } while (rs.getResponseDescription() != "Success");
  }
}