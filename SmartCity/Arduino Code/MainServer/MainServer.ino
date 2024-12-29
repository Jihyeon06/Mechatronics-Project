#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // 그래픽 라이브러리 불러오기
#include <Adafruit_ST7735.h> // ST7735용 라이브러리 불러오기 
#include <SPI.h>             //SPI 라이브러리 불러오기

#define TFT_RST 50  // RST 핀
#define TFT_DC 51   // DC 핀
#define TFT_CS 52   // CS 핀 (필요한 경우)
#define TFT_BLK 53  // BLK 핀 (백라이트 제어)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); // CS 핀을 사용하는 경우

#define ST7735_BLACK 0x0000
#define ST7735_WHITE 0xFFFF
#define ST7735_RED   0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE  0x001F
#define ST7735_CYAN  0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_ORANGE 0xFD20

SoftwareSerial hc12(12, 13); // TX, RX

const int switch_1_Pin = 5;
const int switch_2_Pin = 4;
const int switch_3_Pin = 3;
const int switch_4_Pin = 2;

const int switch_led_1_Pin = 10;
const int switch_led_2_Pin = 9;
const int switch_led_3_Pin = 8;
const int switch_led_4_Pin = 7;

const int signal_power_Pin = 11;

const int Illuminance_Sensor_Pin = A0;

int switch_1_State = 0;
int switch_2_State = 0;
int switch_3_State = 0;
int switch_4_State = 0;

// 신호등 1 (A) 핀 번호
const int redA = 22;
const int yellowA = 24;
const int greenA = 26;

// 신호등 2 (B) 핀 번호
const int redB = 23;
const int yellowB = 25;
const int greenB = 27;

// 상태를 나타내는 상수
enum TrafficState { RED, GREEN, YELLOW };

// 상태 변수
TrafficState stateA = RED;
TrafficState stateB = GREEN;

// 시간 변수
unsigned long previousMillis = 0;
const unsigned long redTime = 2400;    // 빨강 지속 시간 (24초)
const unsigned long greenTime = 2400;  // 초록 지속 시간 (24초)
const unsigned long yellowTime = 500; // 노랑 지속 시간 (5초)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  hc12.begin(9600);

  // 텍스트 표시
  tft.initR(INITR_144GREENTAB); 
  tft.fillScreen(ST77XX_BLACK);

  // 사각형 그리기
  //tft.drawRect(20, 30, 100, 50, ST7735_RED); // 빨간색 사각형 그리기

  pinMode(switch_1_Pin, INPUT);
  pinMode(switch_2_Pin, INPUT);
  pinMode(switch_3_Pin, INPUT);
  pinMode(switch_4_Pin, INPUT);
  pinMode(switch_led_1_Pin, OUTPUT);
  pinMode(switch_led_2_Pin, OUTPUT);
  pinMode(switch_led_3_Pin, OUTPUT);
  pinMode(switch_led_4_Pin, OUTPUT);

  pinMode(signal_power_Pin, OUTPUT);

  // 신호등 A 핀 초기화
  pinMode(redA, OUTPUT);
  pinMode(yellowA, OUTPUT);
  pinMode(greenA, OUTPUT);
  
  // 신호등 B 핀 초기화
  pinMode(redB, OUTPUT);
  pinMode(yellowB, OUTPUT);
  pinMode(greenB, OUTPUT);

  updateTrafficLights();

  pinMode(6, OUTPUT); // 트렌지스터 연결핀 (led 조작)
}

void loop() {
  int Illuminance_Sensor_value = map(analogRead(Illuminance_Sensor_Pin), 100, 1024, 0, 255);
  Serial.println(Illuminance_Sensor_value);

  // 텍스트
  tft.fillScreen(ST7735_BLACK); // 화면을 검정색으로 채우기
  tft.setTextColor(ST7735_WHITE); // 텍스트 색상 설정 (흰색)
  tft.setTextSize(2); // 텍스트 크기 설정
  tft.setCursor(10, 10); // 텍스트 시작 위치 설정
  tft.println("Hello"); // "Hello" 출력
  
  // 스위치 
  switch_1_State = digitalRead(switch_1_Pin);
  switch_2_State = digitalRead(switch_2_Pin);
  switch_3_State = digitalRead(switch_3_Pin);
  switch_4_State = digitalRead(switch_4_Pin);

  analogWrite(signal_power_Pin, 100);

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
    if (switch_3_State == HIGH) {
      if (Illuminance_Sensor_value > 100)
      {
        analogWrite(6, 255);
      }
      else {
        analogWrite(6,0);
      }
    }
  }
  else { 
    analogWrite(6, 0);
  }

  while (Serial.available()) {
    char sendChar = Serial.read(); // 한 문자씩 읽기
    hc12.print(sendChar);          // 한 문자씩 송신
  }

  unsigned long currentMillis = millis();

  // A의 상태에 따른 B의 상태 전환
  if (stateA == GREEN && currentMillis - previousMillis >= greenTime) {
    previousMillis = currentMillis;
    stateA = YELLOW;
    stateB = RED;
  } 
  else if (stateA == YELLOW && currentMillis - previousMillis >= yellowTime) {
    previousMillis = currentMillis;
    stateA = RED;
    stateB = GREEN;
  } 
  else if (stateA == RED && currentMillis - previousMillis >= redTime) {
    previousMillis = currentMillis;
    stateA = GREEN;
    stateB = RED;
  }

  // B의 상태에 따른 A의 상태 전환
  if (stateB == GREEN && currentMillis - previousMillis >= greenTime) {
    previousMillis = currentMillis;
    stateB = YELLOW;
    stateA = RED;
  } 
  else if (stateB == YELLOW && currentMillis - previousMillis >= yellowTime) {
    previousMillis = currentMillis;
    stateB = RED;
    stateA = GREEN;
  }
  else if (stateB == RED && currentMillis - previousMillis >= redTime) {
    previousMillis = currentMillis;
    stateB = GREEN;
    stateA = RED;
  }

  // 상태에 따라 LED 업데이트
  updateTrafficLights();
}

void updateTrafficLights() {
  // 신호등 A
  digitalWrite(redA, stateA == RED);
  digitalWrite(yellowA, stateA == YELLOW);
  digitalWrite(greenA, stateA == GREEN);

  // 신호등 B
  digitalWrite(redB, stateB == RED);
  digitalWrite(yellowB, stateB == YELLOW);
  digitalWrite(greenB, stateB == GREEN);
}