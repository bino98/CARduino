import processing.serial.*;

Serial carduino;

void setup() {
  /**  
   "/dev/tty.***" は環境に合わせて変えてください
   CARduinoのボーレートは38400
   **/
  carduino = new Serial(this, "/dev/tty.CARduino-DevB", 38400);
}

void draw() {
}

void keyPressed() {

  // 6つ目のLEDだけ赤くする．
  if (key == '1') {
    sendCarduino("s,5,100,0,0.");
  }

  // 6つ目のLEDだけ青くする．
  if (key == '2') {
    sendCarduino("s,5,0,0,100.");
  }

  // 全てのLEDを緑にする．
  if (key == '3') {
    sendCarduino("s,a,0,100,0.");
  }

  // 全てのLEDを白にする．
  if (key == '4') {
    sendCarduino("s,a,100,100,100.");
  }
  
  // 加速度センサ，温度センサの値を取得．
  if (key == '5') {
    sendCarduino("d.");
  }
}

void serialEvent(Serial p) {
  String inString = p.readString();

  // 送られてきたメッセージをコンソールに表示
  print(inString);
}

void sendCarduino(String msg) {
  char sendStr[] = msg.toCharArray();
  for (int i=0; i<sendStr.length; i++) {
    carduino.write(sendStr[i]);
    delay(100); // required. BT通信に必要な遅延です．
  }
}
