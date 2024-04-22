/**
 * SDA -> A4
 * SCL -> A5
**/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(19200);
  delay(200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  if (!mpu.begin()) {
    digitalWrite(2, HIGH);
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  //mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);

/*
  mpu.setMotionDetectionThreshold(2);
  mpu.setMotionDetectionDuration(1);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
*/
  delay(100);
}

float avg = 0.0;


void loop() {
  digitalWrite(2, LOW);
  
/*
  if(mpu.getMotionInterruptStatus()) {
    digitalWrite(2, HIGH);
    Serial.println("D");
    delay(1);
  }
*/

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float data = a.acceleration.y;

  if (data < -12) {
    digitalWrite(2, HIGH);
    Serial.println(a.acceleration.y);
    delay(100);
  }

/*
  Serial.print(millis());
  Serial.print(",");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print(a.acceleration.z);
  Serial.println("");


  Serial.print(a.acceleration.z);
  Serial.print(" ");
  Serial.print(g.gyro.z);
  Serial.print(temp.temperature);
  Serial.println("");
  delay(10);
*/
}
