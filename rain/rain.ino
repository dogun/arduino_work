#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
 
Adafruit_7segment matrix = Adafruit_7segment();

float h = 0;
int l_pin = 2;
int r_pin = 4;

bool start = false;

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
#endif
  matrix.begin(0x70);
  //330ml 45下
  //漏斗直径10cm
  //计算面积
  float s = PI * 25;
  //计算每次的体积
  float x = (float)330 / 50;
  //计算每次的水量高度，单位mm
  h = x / s * 10;
}

bool read_status(int pin) {
  int n = digitalRead(pin);
  Serial.print(pin);
  Serial.print(":");
  Serial.println(n);
  if (n > 0) { //大于2v电压
    return true;
  }else {
    return false;
  }
}

bool l_pre_status = false;
bool r_pre_status = false;

float hight = 0;

void loop() {
  int t = millis() / 1000;
  bool s = read_status(l_pin);
  if (s == true && l_pre_status == false) { //增加一次
    if (start == false) {
      start = true;
    }else {
      hight += h;
      Serial.println("l add");
    }
  }
  l_pre_status = s;
  
  s = read_status(r_pin);
  if (s == true && r_pre_status == false) {
    if (start == false) {
      start = true;
    }else {
      hight += h;
      Serial.println("r add");
    }
  }
  r_pre_status = s;

  matrix.print(hight);
  matrix.writeDisplay();
  delay(500);
}
