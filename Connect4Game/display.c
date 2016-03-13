#include "display.h"

int drawColumn;

byte display[8][8];
byte greenColumns[8];
byte redColumns[8];

unsigned long nextDisplayDrawAllowed;

#define FPS (100)
#define MICROS (1000000LL / (FPS*8))

void paintDisplayBuffer() {
  for (byte x = 0; x < 8; x++) {
    byte *green = &greenColumns[x];
    byte *red = &redColumns[x];
    *green = 0;
    *red = 0;
    for (byte y = 0; y < 8; y++) {
      byte cell = display[x][y];
      if (cell & GREEN) {
        *green |= 1 << y;
      }
      if (cell & RED) {
        *red |= 1 << y;
      }
    }
  }
}

void drawDisplay() {
  unsigned long timeMicros = micros();

  if (timeMicros < nextDisplayDrawAllowed) {
    return;
  }

  nextDisplayDrawAllowed = timeMicros + MICROS;

  if (drawColumn == 0)
    paintDisplayBuffer();

  //green
  shiftOut(dataPin, clockPin, MSBFIRST, ~greenColumns[drawColumn]);

  //red
  shiftOut(dataPin, clockPin, LSBFIRST, ~redColumns[drawColumn]);

  //column
  shiftOut(dataPin, clockPin, MSBFIRST, 1 << drawColumn);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(latchPin, LOW);

  drawColumn = (drawColumn + 1) % 8;
}

void clearDisplay(byte colour) {
  for (byte y = 0; y < 8; y++)
    for (byte x = 0; x < 8; x++)
      display[x][y] = colour;
}

void drawDelay(int milliseconds) {
  unsigned long end = millis() + milliseconds;
  while (millis() < end) {
    drawDisplay();
  }
}

void stopDisplay() {
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);

  digitalWrite(latchPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(latchPin, LOW);
}

