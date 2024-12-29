#include <SoftwareSerial.h>
SoftwareSerial hc12(2, 3); // TX, RX

int button_pin = 4;
int running = 0;

char codekey[] = "20241226";

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);

  pinMode(button_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(button_pin) == 1 && running == 0)
  {
    running = 1;
    Serial.println("Luanch Start!!");
    Serial.println(codekey); // 디버깅 용도
    hc12.println(codekey);
  }

  if (Serial.available()) {
    String input = Serial.readString();
    input.trim(); // 공백 및 개행 문자 제거
    hc12.println(input);
    Serial.println(input);
  }

  delay(100);
}
