#include <SoftwareSerial.h>

SoftwareSerial hc12(2, 3); // TX, RX

const int signal_r_light= 9;
const int signal_y_light= 10;
const int signal_g_light= 11;

const int signal_power_Pin = A0;

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);

  pinMode(signal_r_light, OUTPUT);
  pinMode(signal_y_light, OUTPUT);
  pinMode(signal_g_light, OUTPUT);
}

void loop() {
  int signal_power_Value = map(analogRead(signal_power_Pin), 0, 1024, 0, 255);
  Serial.println(signal_power_Value);
  /*
  digitalWrite(signal_r_light, HIGH);
  digitalWrite(signal_y_light, LOW);
  delay(15000); // 10초 대기
  digitalWrite(signal_g_light, HIGH);
  digitalWrite(signal_r_light, LOW);
  delay(10000); // 10초 대기
  digitalWrite(signal_y_light, HIGH);
  digitalWrite(signal_g_light, LOW);
  delay(5000); // 5초 대기
  */
  String receivedData = ""; // 문자열을 저장할 변수 선언

while (hc12.available()) {
    char receivedChar = hc12.read(); // 한 문자씩 읽기
    receivedData += receivedChar;    // 문자열에 문자를 추가
    // 문자열이 "red", "green", "yellow", "off" 중 하나일 경우 처리
    if (receivedData.endsWith("red")) {
        analogWrite(signal_r_light, signal_power_Value);
        analogWrite(signal_g_light, 0);
        analogWrite(signal_y_light, 0);
        receivedData = ""; // 처리 후 문자열 초기화
    } else if (receivedData.endsWith("green")) {
        analogWrite(signal_r_light, 0);
        analogWrite(signal_g_light, signal_power_Value);
        analogWrite(signal_y_light, 0);
        receivedData = ""; // 처리 후 문자열 초기화
    } else if (receivedData.endsWith("yellow")) {
        analogWrite(signal_r_light, 0);
        analogWrite(signal_g_light, 0);
        analogWrite(signal_y_light, signal_power_Value);
        receivedData = ""; // 처리 후 문자열 초기화
    } else if (receivedData.endsWith("off")) {
        analogWrite(signal_r_light, 0);
        analogWrite(signal_g_light, 0);
        analogWrite(signal_y_light, 0);
        receivedData = ""; // 처리 후 문자열 초기화
    }

    Serial.print(receivedChar); // 한 문자씩 출력
  }
}
