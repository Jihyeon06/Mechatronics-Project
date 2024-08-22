#include <SoftwareSerial.h>

SoftwareSerial hc12(2, 3); // TX, RX

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);
}

void loop() {
  while (hc12.available()) {
    char receivedChar = hc12.read(); // 한 문자씩 읽기
    Serial.print(receivedChar);      // 한 문자씩 출력
  }
}