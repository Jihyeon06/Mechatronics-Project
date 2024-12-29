#include <Servo.h>
Servo servo;
int angle = 75;

int speed = 3;
int In1 = 2;
int In2 = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(speed, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  servo.attach(5);

  Serial.println("Enter the u or d");
  Serial.println("u = angle + 15");
  Serial.println("d = angle - 15\n");
}

void loop() {
  if (Serial.available())
  {
    char input = Serial.read();

    if (input == 'a')
    {
      Serial.print("+15");
      for (int i = 0; i < 15; i++)
      {
        angle = angle + 1;
        if (angle >= 180)
          angle = 180;

        servo.write(angle);
        delay(10);
        }
        
        Serial.print("\t\t");
        Serial.println(angle);
      }
      else if (input == 'd')
      {
        Serial.print("\t-15\t");
        for (int i = 0; i < 15; i++)
        {
          angle = angle - 1;
          if (angle <= 0)
            angle = 0;

          servo.write(angle);
          delay(10);
        }
        Serial.println(angle);
      }
      else if (input == 'w')
      {
        Serial.println("forward");
        digitalWrite(In1, HIGH);
        digitalWrite(In2, LOW);
        analogWrite(speed, 255);
      }
      else if (input == 's')
      {
        Serial.println("backward");
        digitalWrite(In1, LOW);
        digitalWrite(In2, HIGH);
        analogWrite(speed, 255);
      }
      else if (input == 'x')
      {
        Serial.println("stop");
        digitalWrite(In1, LOW);
        digitalWrite(In2, LOW);
        analogWrite(speed, 0);
      }
      else
      {
        Serial.println("wrong character!!");
      }
    }
    
  }
