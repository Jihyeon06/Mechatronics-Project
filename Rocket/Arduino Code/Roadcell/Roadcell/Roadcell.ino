#include "HX711.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

File myFile;

#define calibration_factor -15600.0 //-7050.0 // 보정 계수
#define DOUT  3  // HX711 DOUT 핀
#define CLK  2   // HX711 CLK 핀
#define GRAVITY 9.8 // 중력 가속도

unsigned long startTime = 0;
bool running = false;

HX711 scale; // 기본 생성자 사용

int ignitor_relay_pin = 6;
int button_pin = 5;

int buttonState = 0;     // 현재 버튼 상태
int lastButtonState = 0; // 이전 버튼 상태
bool relayState = false;

int fileIndex = 1;
String fileName;

void setup() {
  Serial.begin(9600);

  pinMode(ignitor_relay_pin, OUTPUT);
  pinMode(button_pin, INPUT);

  digitalWrite(ignitor_relay_pin, HIGH);

  Serial.println("HX711 scale TEST");
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); 
  scale.tare(); // 초기 영점 설정
  Serial.println("Readings: ");

  if (!SD.begin(4)) { // SD카드 모듈을 초기화합니다.
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  while (true) {
    fileName = "Test(" + String(fileIndex) + ").txt";
    if (!SD.exists(fileName)) { // 파일이 존재하지 않으면 이 이름을 사용합니다.
      break;
    }
    fileIndex++; // 존재하면 다음 번호로 넘어갑니다.
  }
}

void loop() {
  // 버튼 상태 읽기
  buttonState = digitalRead(button_pin);

  // 버튼이 눌렸다가 떼졌을 때 (토글 동작)
  if (buttonState == HIGH && lastButtonState == LOW) {
    //delay(50); // 디바운싱 처리
    if (digitalRead(button_pin) == HIGH) { // 다시 확인
      relayState = !relayState;
      
      if (relayState == true)
      {
        digitalWrite(ignitor_relay_pin, LOW);

        running = true;
        startTime = millis();
      }
      else if (relayState == false)
      {
        digitalWrite(ignitor_relay_pin, HIGH);

        running = false;
        startTime = 0;
      }
    }
  }
  lastButtonState = buttonState; // 버튼 상태 업데이트
  
  // 무게 읽기 및 출력
  float weight_kg = scale.get_units(); // 질량(kg) 단위로 읽기
  float weight_newton = weight_kg * GRAVITY; // 뉴턴 단위로 변환

  if (running) {
    unsigned long currentTime = millis() - startTime;
    Serial.print(currentTime / 1000.0, 1);
    Serial.print(" s | ");
    Serial.print(weight_kg); // 무게를 kg 단위로 변환하여 출력
    Serial.print(" kg | ");
    Serial.print(weight_newton); // 무게를 kg 단위로 변환하여 출력
    Serial.println(" N");
    
    // 파일을 생성하고 데이터를 씁니다.
    myFile = SD.open(fileName, FILE_WRITE);

    if (myFile) { // 파일이 정상적으로 열리면 파일에 문자를 작성(추가)합니다.
      myFile.print(currentTime / 1000.0, 1);
      myFile.print(" s | ");
      myFile.print(weight_kg); // 무게를 kg 단위로 변환하여 출력
      myFile.print(" kg | ");
      myFile.print(weight_newton); // 무게를 kg 단위로 변환하여 출력
      myFile.println(" N");
      myFile.close(); // 파일을 닫습니다.
    } else {
      // 파일이 열리지 않으면 에러를 출력합니다.
      Serial.println("error opening " + fileName);
    }
  }
  else {
    Serial.print(weight_kg); // 무게를 kg 단위로 변환하여 출력
    Serial.print(" kg | ");
    Serial.print(weight_newton); // 무게를 kg 단위로 변환하여 출력
    Serial.println(" N");
  }
  delay(100); // 루프 간 딜레이
}