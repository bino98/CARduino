#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define LEDPIN            9
#define NUMPIXELS      10
#define RxD 11
#define TxD 12

SoftwareSerial blueToothSerial(RxD,TxD);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
char *receiveDatas[10] = {};

void setup() 
{ 
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
  pixels.begin();
} 

void loop() 
{ 
  char receiveChars[128] = {
  };

  if(blueToothSerial.available())
  {
    for(int i=0; i<sizeof(receiveChars) / sizeof(receiveChars[0]); i++) {
      char r = blueToothSerial.read();
      if (r == '.') {
        receiveChars[i] = '\0';
        break;
      }
      if((r >= '0' && r <= '9') || (r >= 'a' && r <= 'z') || r == '.' || r == ',')
      {
        receiveChars[i] = r;
      } 
      else {
        i--;
      }

    }
    blueToothSerial.flush();
    char *tok;
    tok = strtok(receiveChars, ",");
    for(int i=0; tok != NULL || i < 10; i++) {
      receiveDatas[i] = tok;
      tok = strtok(NULL, ",");
    }
  }

  if(*receiveDatas[0] == 'd') {
    int x = analogRead(A0);
    int y = analogRead(A1);
    int z = analogRead(A2);
    int t = analogRead(A3);

    String sendStr = String(x)  + ',' + String(y) + ',' + String(z) + ',' + String(t);

    blueToothSerial.print("d," + sendStr + ".\r\n");
  } 
  else if(*receiveDatas[0] == 's') {
    
    char *num = receiveDatas[1];

    int r,g,b;
    r = atoi(receiveDatas[2]);
    g = atoi(receiveDatas[3]);
    b = atoi(receiveDatas[4]);
    
    if(*num == 'a') {
      changeAllColor(r, g, b);
      blueToothSerial.print("s,a,OK.\r\n");
    } 
    else {
      changeColor(atoi(num), r, g, b);
      blueToothSerial.print("s," + String(num) + ",OK.\r\n");
    }
  }

} 

void changeColor(int num, int r, int g, int b) {
  uint32_t oldColor[NUMPIXELS];
  for(int i=0;i<NUMPIXELS;i++){
    oldColor[i] = pixels.getPixelColor(i);
    if(num == i){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
    } else {
      pixels.setPixelColor(i, oldColor[i]);
    }
    pixels.show();
  }
}

void changeAllColor(int r, int g, int b) {
  for(int i=0;i<NUMPIXELS;i++){
    changeColor(i, r, g, b);
  }
}


void setupBlueToothConnection()
{
  blueToothSerial.begin(38400);
  delay(1000);
  sendBlueToothCommand("\r\n+STWMOD=0\r\n");
  sendBlueToothCommand("\r\n+STBD=38400\r\n");
  sendBlueToothCommand("\r\n+STNA=CARduino\r\n");
  sendBlueToothCommand("\r\n+STAUTO=0\r\n");
  sendBlueToothCommand("\r\n+STOAUT=1\r\n");
  sendBlueToothCommand("\r\n +STPIN=0000\r\n");
  delay(2000); // This delay is required.
  sendBlueToothCommand("\r\n+INQ=1\r\n");
  delay(2000); // This delay is required.
}

void CheckOK()
{
  char a,b;
  while(1)
  {
    if(blueToothSerial.available())
    {
      a = blueToothSerial.read();

      if('O' == a)
      {
        while(blueToothSerial.available()) 
        {
          b = blueToothSerial.read();
          break;
        }
        if('K' == b)
        {
          break;
        }
      }
    }
  }

  while( (a = blueToothSerial.read()) != -1){}
}

void sendBlueToothCommand(char command[])
{
  blueToothSerial.print(command);
  CheckOK();   
}

