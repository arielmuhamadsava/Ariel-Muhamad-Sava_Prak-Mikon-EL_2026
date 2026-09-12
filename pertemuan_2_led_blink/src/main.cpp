#include <Arduino.h>

const int LedPin = 16;

void setup() {
  // put your setup code here, to run once:
  // Mengatur pin LED sebagai OUTPUT
  pinMode(LedPin, OUTPUT);
  
  // Memulai komunikasi serial untuk debugging
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LedPin, HIGH);  // Menyalakan LED
  Serial.println("LED Menyala");
  delay(3000);                 // Menunggu 3 detik
  
  digitalWrite(LedPin, LOW);   // Mematikan LED
  Serial.println("LED Mati");
  delay(3000);                 // Menunggu 3 detik
}

