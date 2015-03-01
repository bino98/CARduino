#include <Adafruit_NeoPixel.h>
#define PIN            9
#define NUMPIXELS      10
#define NUMREC         20
#define NUMAVEREC      10
#define DIFF           80

int zRecAcels[NUMREC];
int aveZRecAcels[NUMAVEREC];
int zAcelRecsCounter = 0;
int aveZAcelRecsCounter = 0;

int UPDIFF = 20;
int DOWNDIFF = 20;

int rColor, bColor, gColor = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  Serial.begin(9600);

  zAcelRecsCounter = 0;
  for(int i=0; i<NUMREC; i++){
    zRecAcels[i] = 0;
  }
}

void loop() {
  int zAcel = analogRead(A2);
  
  // zAcelをレコードする
  if(zAcelRecsCounter >= NUMREC) zAcelRecsCounter = 0;
  zRecAcels[zAcelRecsCounter] = zAcel;
  zAcelRecsCounter++;
  
  // zAcelの平均を算出
  int f = 0;
  for(int i=0; i<zAcelRecsCounter; i++){
    f = f + zRecAcels[i];
  }
  int aveZRecAcel = f / zAcelRecsCounter;
  
  // aveZAcelをレコードする
  if(aveZAcelRecsCounter >= NUMAVEREC) aveZAcelRecsCounter = 0;
  aveZRecAcels[aveZAcelRecsCounter] = aveZRecAcel;
  aveZAcelRecsCounter++;
  
  //aveの最大値，最小値を出す
  int aveZMax = 0;
  for(int i=0; i<NUMAVEREC; i++){
    if(aveZMax < aveZRecAcels[i]) {
      aveZMax = aveZRecAcels[i];
    }
  }
  
  int aveZMin = 1024;
  for(int i=0; i<NUMAVEREC; i++){
    if(aveZMin > aveZRecAcels[i]) {
      aveZMin = aveZRecAcels[i];
    }
  }
  
  
  if(zAcel > aveZMax + DOWNDIFF && bColor <= 0) {
    if(rColor <= 200) rColor = rColor + 50;
    bColor = 0;
  } else if(rColor >= 5) {
    rColor = rColor - 5;
    UPDIFF = DIFF;
  }
  
  if(zAcel < aveZMin - UPDIFF && rColor <= 0) {
    if(bColor <= 200) bColor = bColor + 50
    ;
    rColor = 0;
  } else if(bColor >= 5) {
    bColor = bColor - 5;
    DOWNDIFF = DIFF;
  }

  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(rColor,gColor,bColor));
    pixels.show();
  }
  delay(20);
  
  Serial.println("aveZRecAcel:" + String(aveZRecAcel));
  Serial.println("aveZMax:" + String(aveZMax));
  Serial.println("aveZMin:" + String(aveZMin));
  Serial.println();
}

int averageVal(int r[]) {
  int f = 0;
  int num = sizeof(r)/sizeof(r[0]);
  for(int i=0; i<num; i++){
    f = f + r[i];
  }
  f = f / num;
}

int maxSearch(int r[]) {
  int num = sizeof(r)/sizeof(r[0]);
  int m = 0;
  for(int i=0; i<num; i++){
    if(m < r[i]) {
      m = r[i];
    }
  }
  return m;
}

int minSearch(int r[]) {
  int num = sizeof(r)/sizeof(r[0]);
  int m = 1024;
  for(int i=0; i<num; i++){
    if(m > r[i]) {
      m = r[i];
    }
  }
  return m;
}
