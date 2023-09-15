/* ArduinoProMini-6通道接收机 
 *     by Bilibili 蔡子CaiZi
 *     
 * PWM输出 -> 引脚2~6、9
 * 
 * NRF24L01 | Arduino
 * CE    -> 7
 * CSN   -> 8
 * MOSI  -> 11
 * MISO  -> 12
 * SCK   -> 13
 * IRQ   -> 无连接
 * VCC   -> 小于3.6V
 * GND   -> GND
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

struct Signal {
  byte on1;
  byte on2;
  byte on3;
  byte on4;
  byte on5;
  byte on6;
  byte on7;
};

Signal data;
uint8_t address[][6] = {"12345"};
RF24 radio(D7, D8);

void ResetData() 
{
  data.on1 = 0;
  data.on2 = 0;
  data.on3 = 0;
  data.on4 = 0;
  data.on5 = 0;
  data.on6 = 0;
  data.on7 = 0;
}

void setup()
{
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D9,OUTPUT);
  pinMode(D10,OUTPUT);
  digitalWrite(D2, 0);
  digitalWrite(D3, 0);
  digitalWrite(A1, 0);
  digitalWrite(D5, 0);
  digitalWrite(D6, 0);
  digitalWrite(D9, 0);
  digitalWrite(D10, 0);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address[0]);
  radio.startListening();
  ResetData();
  
  printf_begin();
  radio.printPrettyDetails();
}

void recvData()
{
  while ( radio.available() ) {
    radio.read(&data, sizeof(Signal));//接收数据
  }
}
void loop()
{
  recvData();
  Serial.print("\t");Serial.print(data.on1);
  Serial.print("\t");Serial.print(data.on2);
  Serial.print("\t");Serial.print(data.on3);
  Serial.print("\t");Serial.print(data.on4);
  Serial.print("\t");Serial.print(data.on5);
  Serial.print("\t");Serial.print(data.on6);
  Serial.print("\t");Serial.println(data.on7);

  digitalWrite(D2, data.on1);
  digitalWrite(D3, data.on2);
  digitalWrite(A1, data.on3);
  digitalWrite(D5, data.on4);
  digitalWrite(D6, data.on5);
  digitalWrite(D9, data.on6);
  digitalWrite(D10, data.on7);
}
