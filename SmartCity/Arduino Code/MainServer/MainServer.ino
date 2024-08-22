#include <SoftwareSerial.h>

SoftwareSerial hc12(2, 3); // TX, RX

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);
}

void loop() {
  while (Serial.available()) {
    char sendChar = Serial.read(); // 한 문자씩 읽기
    hc12.print(sendChar);          // 한 문자씩 송신
  }
}