#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();
const int ML_PIN = D3;
const int MR_PIN = D5;
const int MZ_PIN = D6;

int x = 0;
int y = 0;
int z = 0;

int px = 0;
int py = 0;
int pz = 0;

void setup() {
  matrix.begin(0x70);
  Serial.begin(9600);
  pinMode(ML_PIN, OUTPUT);
  pinMode(MR_PIN, OUTPUT);
  pinMode(MZ_PIN, OUTPUT);
}

int find_num(String d, String s, String e = " ") {
  int is = d.indexOf(s);
  if (is < 0) return -1;
  d = d.substring(is + s.length());
  int ie = d.indexOf(e);
  if (ie < 0) return -1;
  d = d.substring(0, ie);
  return d.toInt();
}

const int AVG_TIMES = 10;
int _cnt = 0;
int _xyz[3] = {0, 0, 0};
int* mean_value_filter(int x, int y, int z) {
  _xyz[0] = (_xyz[0] * _cnt + x) / (_cnt + 1);
  _xyz[1] = (_xyz[1] * _cnt + y) / (_cnt + 1);
  _xyz[2] = (_xyz[2] * _cnt + z) / (_cnt + 1);
  _cnt += 1;
  if (_cnt > AVG_TIMES) {
    _cnt = 0;
    return _xyz;
  } else {
    return NULL;
  }
}

bool read_xyz(bool test = false) {
  if (Serial.available() > 0) {
    String d = "";
    while (Serial.available() > 0) {
      d += char(Serial.read());
      delay(2);
    }
    if (d.length() > 0) {
      Serial.print(d);
      int ix, iy, iz;
      if (!test) {
        ix = find_num(d, "X = ");
        iy = find_num(d, "Y = ");
        iz = find_num(d, "Z = ");
      } else {
        ix = iy = iz = find_num(d, "DIST = ");
      }

      int* res = mean_value_filter(ix, iy, iz);

      if (res != NULL) {
        px = x;
        py = y;
        pz = z;

        x = res[0];
        y = res[1];
        z = res[2];
        return true;
      }
    }
  }
  return false;
}

int _ML_SPEED = 0;
int _MR_SPEED = 0;
int _MZ_SPEED = 0;
const int MAX_SPEED = 40;
const int SPEED_STEP = 10;

int P[3] = {100, 200, 100};
void gogogo(int x, int y, int z, int p[3]) {
  int gx = p[0];
  int gy = p[1];
  int gz = p[2];
  //z
  if (z < gz && _MZ_SPEED <= MAX_SPEED - SPEED_STEP) {
    _MZ_SPEED += SPEED_STEP;
  }else if (z > gz && _MZ_SPEED >= SPEED_STEP){
    _MZ_SPEED -= SPEED_STEP;
  }
  analogWrite(MZ_PIN, _MZ_SPEED);

  //x y
  
}

void _PROCESS() {
  gogogo(x, y, z, P);
}

void show_data() {
  int n = (int)(millis() / 100) % 20;
  if (n <= 5) matrix.print(1000 + x);
  else if (n <= 10) matrix.print(2000 + y);
  else if (n <= 15) matrix.print(3000 + z);
  else matrix.print(4000 + _MZ_SPEED);
  matrix.writeDisplay();
}

void loop() {
/*
  int ii = 0;
  while (true) {
    if (ii > 255) ii = 0;
    analogWrite(MZ_PIN, ii);
    delay(300);
    matrix.print(ii);
    matrix.writeDisplay();
    ii++;
  }
*/
  bool r = read_xyz(true);
  if (r) _PROCESS();
  show_data();
}
