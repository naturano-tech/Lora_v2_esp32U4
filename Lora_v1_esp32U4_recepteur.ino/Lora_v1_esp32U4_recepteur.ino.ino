#include "Arduino.h"
#include "LoRa_E32.h"

// Configuration des broches pour ESP32 38 pins
LoRa_E32 e32ttl(&Serial2, 15, 4, 2); // RX, AUX, M0, M1

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialisation du module LoRa
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  e32ttl.begin();

  Serial.println("Module LoRa initialisé - Prêt à envoyer des messages");
}

void loop() {

  if (e32ttl.available() > 1) {
    ResponseContainer rc = e32ttl.receiveMessage();
    if (rc.status.code != 1) {
      Serial.println(rc.status.getResponseDescription());
    } else {
      Serial.print("Message reçu: ");
      Serial.println(rc.data);
    }
  }

  // Envoi de message depuis le moniteur série
  if (Serial.available()) {
    String input = Serial.readString();
    e32ttl.sendMessage(input);
  }
}