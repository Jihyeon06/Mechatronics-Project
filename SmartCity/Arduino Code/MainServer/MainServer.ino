#include <SoftwareSerial.h>

SoftwareSerial hc12(0, 1); //TX,RX

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);
}

void loop() {
  hc12.print(1);
  hc12.println("");

  Serial.print(1);
  Serial.println("");

  delay(10);
}