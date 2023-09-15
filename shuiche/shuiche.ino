const int STEP_PIN = D3;
const int DIR_PIN = D5;
const int EN_PIN = D2;

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
  digitalWrite(DIR_PIN, LOW);
  delay(1);
  digitalWrite(EN_PIN, LOW);
}

void run_loop(int hz) {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(1000000 / hz / 2);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(1000000 / hz / 2);
}

void loop() {
  run_loop(38);
  //analogWrite(ML_PIN, 128);
}
