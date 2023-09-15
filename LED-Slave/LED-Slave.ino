#include <Wire.h>

const int MY_ID = 100;

const int MAX = 100;
const int PIN_NUM = 10;

int out_pins[PIN_NUM] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
float pins_duty[PIN_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
  int i;
  for (i = 0; i < PIN_NUM; i++) {
    pinMode(out_pins[i], OUTPUT);
    digitalWrite(out_pins[i], LOW);
  }
  Wire.begin(MY_ID);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

int c = 0;
int writec = 0;
void loop()
{
  int i;
  for (i = 0; i < 10; i++) {
    int pin = out_pins[i];
    float duty = pins_duty[i];
    if (c == 0 && duty > 0) {
      digitalWrite(pin, HIGH);
    }
    if (duty == c) {
      digitalWrite(pin, LOW);
    }
  }
  c++;
  if (c >= MAX) c = 0;
  //delayMicroseconds(1);
}

void receiveEvent(int howMany)
{
  Serial.println("receive...");
  while(1 < Wire.available())
  {
    char c = Wire.read();
    if (c == 0) {
      char num = Wire.read();
      char duty = Wire.read();
      if (num < 0 || num > 9) {
        Serial.print("set duty error: pin:");
        Serial.print((int)num);
        Serial.print(", duty:");
        Serial.println((int)duty);
        break;
      }
      pins_duty[num] = duty;
      Serial.print("set duty, pin:");
      Serial.print((int)num);
      Serial.print(", duty:");
      Serial.println((int)duty);
    }
  }
}
