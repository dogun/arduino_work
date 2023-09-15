const int HO_PIN = 12;
const int MO_PIN = 11;

const int P0 = 2;
const int P1 = 3;
const int P2 = 4;
const int P3 = 5;
const int P5 = 6;

const int TEM_PIN = A1;
const int W_PIN = A2;
const int O_PIN = 12;

const int BTN_5 = 7;
const int BTN_10 = 8;
const int BTN_15 = 9;

const int BTN_MO = 10;

void setup() {
  Serial.begin(9600);
  
  pinMode(W_PIN, INPUT);
  pinMode(O_PIN, INPUT);
  pinMode(BTN_5, INPUT);
  pinMode(BTN_10, INPUT);
  pinMode(BTN_15, INPUT);
  pinMode(TEM_PIN, INPUT);
  pinMode(BTN_MO, INPUT);
  
  pinMode(HO_PIN, OUTPUT);
  pinMode(MO_PIN, OUTPUT);
  stop_ho();
  stop_mo();

  pinMode(P0, OUTPUT);
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P5, OUTPUT);
  stop_snd();
}

void start_ho() {
  digitalWrite(HO_PIN, LOW);
}

void stop_ho() {
  digitalWrite(HO_PIN, HIGH);
}

void start_mo() {
  digitalWrite(MO_PIN, LOW);
}

void stop_mo() {
  digitalWrite(MO_PIN, HIGH);
}

void stop_snd() {
  play_snd(0, -1);
}

int last_ply = -1;
int ply_cnt = -1;
int ply_cnt_now = 0;
void chk_snd_cnt() {
  if (ply_cnt < 0) return;
  ply_cnt_now++;
  if (ply_cnt_now > ply_cnt) {
    Serial.println("stop play");
    stop_snd();
    ply_cnt = -1;
    ply_cnt_now = 0;
  }
}
void play_snd(int snd_id, int pc) {
  if (snd_id == last_ply) return;
  last_ply = snd_id;
  ply_cnt = pc;
  ply_cnt_now = 0;
  /*
  Serial.println("aaa");
  Serial.println(snd_id);
  Serial.print("P0:");
  Serial.println(((snd_id >> 0) & 1) ? LOW : HIGH);
  Serial.print("P1:");
  Serial.println(((snd_id >> 1) & 1) ? LOW : HIGH);
  Serial.print("P2:");
  Serial.println(((snd_id >> 2) & 1) ? LOW : HIGH);
  Serial.print("P3:");
  Serial.println(((snd_id >> 3) & 1) ? LOW : HIGH);
  Serial.print("P5:");
  Serial.println(((snd_id >> 5) & 1) ? LOW : HIGH);
  */
  snd_id = snd_id & B00011111;
  digitalWrite(P0, ((snd_id >> 0) & 1) ? LOW : HIGH);
  digitalWrite(P1, ((snd_id >> 1) & 1) ? LOW : HIGH);
  digitalWrite(P2, ((snd_id >> 2) & 1) ? LOW : HIGH);
  digitalWrite(P3, ((snd_id >> 3) & 1) ? LOW : HIGH);
  digitalWrite(P5, ((snd_id >> 5) & 1) ? LOW : HIGH);
}

int get_tem() {
  int t = analogRead(TEM_PIN);
  return t;
}

bool get_open_status() {
  return digitalRead(O_PIN);
}

bool get_water_status() {
  return digitalRead(W_PIN);
}

bool get_btn_5_status() {
  bool r = digitalRead(BTN_5);
  //Serial.println(r);
  return r;
}

bool get_btn_10_status() {
  return digitalRead(BTN_10);
}

bool get_btn_15_status() {
  return digitalRead(BTN_15);
}

bool get_btn_mo_status() {
  return digitalRead(BTN_MO);
}

const int MIN_SEC = 60;
const int MO_10 = 10;
//const int MIN_SEC = 1;  //TEST
//const int MO_10 = 1;    //TEST

const int HO_OK_TEM = 94; //温度到多少以上
const int HO_OK_S = 7;  //温度多久不动认为是烧开水
//const int HO_OK_TEM = 45; //TEST
//const int HO_OK_S = 3;  //TEST

const int MO_1_5 = 5 * MO_10;
const int SL_1_5 = 5 * MIN_SEC;
const int MO_2_5 = 1.5 * MO_10;
const int SL_2_5 = 18 * MIN_SEC;

const int MO_1_10 = MO_1_5 * 2;
const int SL_1_10 = SL_1_5 * 2;
const int MO_2_10 = MO_2_5 * 2;
const int SL_2_10 = SL_2_5 * 2;

const int MO_1_15 = MO_1_5 * 3;
const int SL_1_15 = SL_1_5 * 3;
const int MO_2_15 = MO_2_5 * 3;
const int SL_2_15 = SL_2_5 * 3;

const int STATUS_WAIT_INPUT =     B000010;//启动，等待选择制作容量
const int STATUS_HO =             B000011;//正在加热
const int STATUS_HO_OK_MO_1 =     B000100;//加热完成，请加入xxx，并按搅拌按钮
const int STATUS_MO_1 =           B000101;//正在搅拌
const int STATUS_MO_1_OK_SL_1 =   B000110;//搅拌完成，请等待静置
const int STATUS_SL_1_OK_MO_2 =   B000111;//静置完成，请加入xxx，并按搅拌按钮
const int STATUS_MO_2 =           B100101;//正在搅拌,&运算后要和MO_1一样
const int STATUS_MO_2_OK_SL_2 =   B001000;//第二次搅拌完成，请等待静置
const int STATUS_FIN =            B001001;//制作完成
const int STATUS_ERR =            B000001;//出现错误

int MO_1 = 0;
int SL_1 = 0;
int MO_2 = 0;
int SL_2 = 0;

int status = STATUS_WAIT_INPUT;
int ho = 0;
int ho_cnt = 0;
int mo_1_cnt = 0;
int sl_1_cnt = 0;
int mo_2_cnt = 0;
int sl_2_cnt = 0;
void loop() {
  chk_snd_cnt();
  if (status == STATUS_WAIT_INPUT) {
    play_snd(STATUS_WAIT_INPUT, -1); //播放等待输入的声音
    if (get_btn_5_status()) {
      MO_1 = MO_1_5;
      SL_1 = SL_1_5;
      MO_2 = MO_2_5;
      SL_2 = SL_2_5;
      status = STATUS_HO;
    }else if (get_btn_10_status()) {
      MO_1 = MO_1_10;
      SL_1 = SL_1_10;
      MO_2 = MO_2_10;
      SL_2 = SL_2_10;
      status = STATUS_HO;
    }else if (get_btn_15_status()) {
      MO_1 = MO_1_15;
      SL_1 = SL_1_15;
      MO_2 = MO_2_15;
      SL_2 = SL_2_15;
      status = STATUS_HO;
    }
    if (status == STATUS_HO) {
      Serial.println(MO_1);
      ho = 0;
      ho_cnt = 0;
      play_snd(STATUS_HO, 45); //播放正在加热
      start_ho();
    }
  }else if (status == STATUS_HO) {
    delay(1000);
    int _ho = get_tem();
    _ho = _ho / 10;
    Serial.println(_ho);
    if ((_ho == ho || _ho == ho + 1) && _ho >= HO_OK_TEM) {
      ho_cnt += 1;
    } else {
      ho = _ho;
      ho_cnt = 0;
    }
    if (ho_cnt > HO_OK_S) { //烧开了
      Serial.println("烧开了");
      stop_ho(); //停止加热
      status = STATUS_HO_OK_MO_1;
      play_snd(STATUS_HO_OK_MO_1, -1); //播放声音提示放入料按按钮
    }
  }else if (status == STATUS_HO_OK_MO_1) {
    if (get_btn_mo_status()) {
      Serial.println("开始一次搅拌");
      mo_1_cnt = 0;
      status = STATUS_MO_1;
      play_snd(STATUS_MO_1, 45);
      start_mo();//开始搅拌
    }
  }else if (status == STATUS_MO_1) {
    delay(1000);
    mo_1_cnt += 1;
    if (mo_1_cnt > MO_1) { //搅拌1完成
      Serial.println("一次搅拌完成");
      stop_mo(); //停止搅拌
      status = STATUS_MO_1_OK_SL_1;
      play_snd(STATUS_MO_1_OK_SL_1, 45);
      sl_1_cnt = 0;
    }
  }else if (status == STATUS_MO_1_OK_SL_1) {
    delay(1000);
    sl_1_cnt += 1;
    if (sl_1_cnt > SL_1) { //静置1完成
      Serial.println("静置1完成");
      status = STATUS_SL_1_OK_MO_2;
      play_snd(STATUS_SL_1_OK_MO_2, -1);
    }
  }else if (status == STATUS_SL_1_OK_MO_2) {
    if (get_btn_mo_status()) {
      Serial.println("二次搅拌开始");
      mo_2_cnt = 0;
      status = STATUS_MO_2;
      play_snd(STATUS_MO_2, 45);
      start_mo();//开始搅拌2
    }
  }else if (status == STATUS_MO_2) {
    delay(1000);
    mo_2_cnt += 1;
    if (mo_2_cnt > MO_2) { //搅拌2完成
      Serial.println("二次搅拌完成");
      stop_mo(); //停止搅拌
      status = STATUS_MO_2_OK_SL_2;
      play_snd(STATUS_MO_2_OK_SL_2, 45);
      sl_2_cnt = 0;
    }
  }else if (status == STATUS_MO_2_OK_SL_2) {
    delay(1000);
    sl_2_cnt += 1;
    if (sl_2_cnt > SL_2) { //静置2完成
      Serial.println("静置2完成");
      status = STATUS_FIN;
      play_snd(STATUS_FIN, 45);
    }
  }else if (status == STATUS_FIN) {
    delay(1000);
    if (get_btn_mo_status()) { //长按重置
      Serial.println("重置");
      status = STATUS_WAIT_INPUT;
    }
  }
}
