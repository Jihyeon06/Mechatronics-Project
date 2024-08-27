#include "HX711.h"

// HX711 모듈의 DT와 SCK 핀을 연결한 아두이노 핀 번호
#define DOUT  3
#define CLK  2

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  // 초기화와 보정값 설정 (0으로 설정 후 필요시 조정)
  scale.set_scale(2280.f); // 보정값 (로드셀에 따라 조정 필요)
  scale.tare();  // 초기값을 0으로 설정
}

void loop() {
  // 측정된 무게를 시리얼 모니터에 출력
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 1); // 1은 소수점 자리수
  Serial.println(" kg");
  delay(10);  // 1초마다 무게 측정
}
