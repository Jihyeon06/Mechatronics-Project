#include <SoftwareSerial.h>

SoftwareSerial hc12(12, 13); // TX, RX

const int switch_1_Pin = 5;
const int switch_2_Pin = 4;
const int switch_3_Pin = 3;
const int switch_4_Pin = 2;

const int switch_led_1_Pin = 11;
const int switch_led_2_Pin = 10;
const int switch_led_3_Pin = 9;
const int switch_led_4_Pin = 8;

const int Illuminance_Sensor_Pin = A0;

int switch_1_State = 0;
int switch_2_State = 0;
int switch_3_State = 0;
int switch_4_State = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  hc12.begin(9600);
  pinMode(switch_1_Pin, INPUT);
  pinMode(switch_2_Pin, INPUT);
  pinMode(switch_3_Pin, INPUT);
  pinMode(switch_4_Pin, INPUT);
  pinMode(switch_led_1_Pin, OUTPUT);
  pinMode(switch_led_2_Pin, OUTPUT);
  pinMode(switch_led_3_Pin, OUTPUT);
  pinMode(switch_led_4_Pin, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch_1_State = digitalRead(switch_1_Pin);
  switch_2_State = digitalRead(switch_2_Pin);
  switch_3_State = digitalRead(switch_3_Pin);
  switch_4_State = digitalRead(switch_4_Pin);

  Serial.print("1번 : ");
  Serial.print(switch_1_State);
  Serial.print(", 2번 : ");
  Serial.print(switch_2_State);
  Serial.print(", 3번 : ");
  Serial.print(switch_3_State);
  Serial.print(", 4번 : ");
  Serial.println(switch_4_State);

  digitalWrite(switch_led_1_Pin, switch_1_State);
  digitalWrite(switch_led_2_Pin, switch_2_State);
  digitalWrite(switch_led_3_Pin, switch_3_State);
  digitalWrite(switch_led_4_Pin, switch_4_State);

  if (switch_1_State == HIGH) {
    analogWrite(6,255);  
  }
  else { 
    
    analogWrite(6, 0);
  }
  int Illuminance_Sensor_value = map(analogRead(Illuminance_Sensor_Pin), 100, 1024, 10, 0);
  Serial.println(Illuminance_Sensor_value);

  while (Serial.available()) {
    char sendChar = Serial.read(); // 한 문자씩 읽기
    hc12.print(sendChar);          // 한 문자씩 송신
  }
}
