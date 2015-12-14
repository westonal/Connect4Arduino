#include "Arduino.h"
#include "display.h"

int drawColumn;

int display[8][8];
int toggle;

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

void clearDisplay(int colour){
  for(int y=0;y<8;y++)
  for(int x=0;x<8;x++)
  display[x][y]=colour;
}

void drawDelay(int milliseconds){
  long end = millis()+milliseconds;
  while(millis()<end){
    drawDisplay();
  }
}

