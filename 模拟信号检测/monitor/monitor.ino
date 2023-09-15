int monitor_pin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int val=analogRead(monitor_pin);
  float v = (float)val / 4064 * 5;
  delay(50);
  Serial.println(v * 3);
  //Serial.print(" ");
  //Serial.print(val);
}
