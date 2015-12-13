//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;
////Pin connected to DS of 74HC595
int dataPin = 7;

int toggle;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int col = 0; col < 8; col++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    delay(50);
    // shift out the bits:    

    int green = 1<<col;
    int red = 1<<col;
    
    //green
    shiftOut(dataPin, clockPin, MSBFIRST, ~green);
    
    //red
    shiftOut(dataPin, clockPin, MSBFIRST, ~red);
    
    //column
    shiftOut(dataPin, clockPin, MSBFIRST, 1<<col);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(13, toggle==0?HIGH:LOW);
    toggle=(toggle+1)%2;
    //digitalWrite(latchPin, LOW);
    //shiftOut(dataPin, clockPin, MSBFIRST, 255);
    //shiftOut(dataPin, clockPin, MSBFIRST, 255);
    //shiftOut(dataPin, clockPin, MSBFIRST, 1<<row);
    //digitalWrite(latchPin, HIGH);
  }
}
