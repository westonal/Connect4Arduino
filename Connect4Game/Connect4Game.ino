#include <TimeLib.h>

extern "C" {
  #include "display.h"
  #include "pins.h"
  #include "clock.h"
}

int inputs[] = {input_left, input_centre, input_right};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for(int i=0;i<3;i++)
    pinMode(inputs[i], INPUT_PULLUP);

  setTime(1,2,0,1,1,2015);
}

int moveLocation;

int DOWN_LEFT = 1;
int DOWN_RIGHT = 2;
int DOWN_CENTRE = 4;

int mode;

void digitalClockDisplay(){
  int hr = hour();
  int min = minute();

  drawDigit(0,0,hr/10,GREEN,OFF);
  drawDigit(2,0,hr%10,RED,OFF);
  drawDigit(4,0,min/10,GREEN,OFF);
  drawDigit(6,0,min%10,RED,OFF);
}

void loop() {

//  int d = (millis()/500)%2;
//  for(int x=0;x<8;x++)
//    for(int y=1;y<8;y++){
//      display[x][y] = ((x+y)%2==d)?RED:GREEN;
//   }

  clearDisplay(OFF);
  
  if (digitalRead(input_left)==LOW) {
    if ((mode & DOWN_LEFT)==0) {
      moveLocation = (moveLocation + 7)%8;
      setTime((hour()+1%24),minute(),second(),day(),month(),year());
    }
    mode |= DOWN_LEFT;
  } else {
    mode = mode & ~DOWN_LEFT;
  }
  
  if (digitalRead(input_right)==LOW) {
     if ((mode & DOWN_RIGHT)==0) {
       moveLocation = (moveLocation + 1)%8;
       setTime(hour(),(minute()+1)%60,second(),day(),month(),year());
     }
     mode |= DOWN_RIGHT;
  } else {
    mode = mode & ~DOWN_RIGHT;
  }

  if (digitalRead(input_centre)==LOW) {
    moveLocation = 3;
    setTime(hour(),minute(),59,day(),month(),year());
  }

  for(int x=0;x<8;x++)
    display[x][0] = OFF;

  display[moveLocation][0] = ORANGE;

  digitalClockDisplay();
  
  //call often
  drawDisplay();
  //sync before display changes to prevent tearing
  syncDisplay();
}

