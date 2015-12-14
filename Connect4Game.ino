//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 7;

int input_left = 11;
int input_centre = 10;
int input_right = 12;

int inputs[] = {input_left, input_centre, input_right};

int toggle;
int drawColumn;

int display[8][8];

int GREEN = 1;
int RED = 2;
int ORANGE = GREEN | RED;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for(int i=0;i<3;i++)
    pinMode(inputs[i], INPUT_PULLUP);
}

void drawDisplay() {
  int x = drawColumn;  
  int green = 0;
  int red = 0;

  for(int y=0; y<8; y++) {
     int cell = display[x][y];
     if(cell&GREEN){
       green+=1<<y;
     }
     if(cell&RED){
       red+=1<<y;
     }
  }
  
  //green
  shiftOut(dataPin, clockPin, MSBFIRST, ~green);
    
  //red
  shiftOut(dataPin, clockPin, LSBFIRST, ~red);
    
  //column
  shiftOut(dataPin, clockPin, MSBFIRST, 1<<x);
    //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delayMicroseconds(100);
  digitalWrite(13, toggle==0?HIGH:LOW);
  toggle=(toggle+1)%2;
  drawColumn=(drawColumn+1)%8;
}

void syncDisplay() {
  while(drawColumn!=0){
    drawDisplay();
  }
}

void loop() {

  int d = (millis()/500)%2;
  for(int x=0;x<8;x++)
    for(int y=0;y<8;y++){
      display[x][y] = ((x+y)%2==d)?RED:GREEN;
   }

  for (int i=0;i<3;i++){
    if (digitalRead(inputs[i])==LOW) {
      display[i][0] = ORANGE;
    }
  }
  
  //call often
  drawDisplay();
  //sync before display changes to prevent tearing
  syncDisplay();
}

