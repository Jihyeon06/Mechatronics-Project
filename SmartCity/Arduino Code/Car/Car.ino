#include <SoftwareSerial.h>

SoftwareSerial hc12(0, 1); //TX,RX

String input;
int a, b, c, d;
const char coma=',';

void setup() {
  Serial.begin(9600);
  hc12.begin(9600);
}

void loop() {
  while (hc12.available()) {
    input = hc12.readStringUntil('\n');

    if (input.length() > 0){
      a = input.indexOf(coma);
      int X1 = input.substring(0, a).toInt();
    
      Serial.print(X1);
      Serial.println("");
    }
  }
}
