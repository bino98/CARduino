import processing.serial.*;

Serial myPort;

int x;
int y;
int z;
int fillRevise = 500;
int yCoordinateRevise;

int t;

void setup() {
  size(850, 800);

  myPort = new Serial(this, "/dev/tty.SeeeduinoBluetooth-DevB", 38400);
}

void draw() {
  background(255);
//  myPort.write('a');

  fill(x, 0, 0);
  text("x:"+x, 10, height-x-10);
  rect(10, height-x, 200, x);
  fill(0, y, 0);
  text("y:"+y, 220, height-y-10);
  rect(220, height-y, 200, y);
  fill(0, 0, z);
  text("z:"+z, 430, height-z-10);
  rect(430, height-z, 200, z);
  
  fill(0);
  text("temp:"+t, 640, height-t-10);
  rect(640, height-t, 200, t);
  
}

void serialEvent(Serial port) {
  String str = port.readStringUntil('\n');
  if (str != null) {
    str = trim(str);
    int[] values = int(splitTokens(str, ","));
    if(values.length == 4){
      x = int(map(values[0], 300, 750, 0, height));
      y = int(map(values[1], 300, 750, 0, height));
      z = int(map(values[2], 300, 750, 0, height));
      t = int(map(values[3], 480, 440, 0, height));
    }
  }
}

