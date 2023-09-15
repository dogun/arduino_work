
/* ArduinoProMini-6通道发射器 
 *     by Bilibili 蔡子CaiZi
 *     
 * A0~5 -> 模拟输入，2~5 -> 通道正反开关
 * A6 -> 电压检测
 * 6 -> 蜂鸣器
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
#include <printf.h>

uint8_t address[][6] = {"12345"};
RF24 radio(D7, D8); // SPI通信，引脚对应关系：CE ->7,CSN ->8
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
  pinMode(D2,INPUT);
  pinMode(D3,INPUT);
  pinMode(A1,INPUT);
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  pinMode(D9,INPUT);
  pinMode(D10,INPUT);
  
  radio.begin();
  radio.openWritingPipe(address[0]);//pipeOut通信地址
  radio.stopListening(); //发射模式
  ResetData();//初始化6个通道值
  Serial.begin(9600);

  printf_begin();
  radio.printPrettyDetails();
}

void loop()
{
  data.on1 = digitalRead(D2);
  data.on2 = digitalRead(D3);
  data.on3 = digitalRead(A1);
  data.on4 = digitalRead(D5);
  data.on5 = digitalRead(D6);
  data.on6 = digitalRead(D9);
  data.on7 = digitalRead(D10);

  Serial.print("\t");Serial.print(data.on1);
  Serial.print("\t");Serial.print(data.on2);
  Serial.print("\t");Serial.print(data.on3);
  Serial.print("\t");Serial.print(data.on4);
  Serial.print("\t");Serial.print(data.on5);
  Serial.print("\t");Serial.print(data.on6);
  Serial.print("\t");Serial.println(data.on7);
   
  radio.write(&data, sizeof(Signal));//将数据发送出去
}
