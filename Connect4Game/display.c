#include "Arduino.h"
#include "display.h"

int drawColumn;

int display[8][8];
int greenColumns[8];
int redColumns[8];

unsigned long nextDisplayDrawAllowed;

#define FPS (100)
#define MICROS (1000000LL / (FPS*8))

void paintDisplayBuffer() {
  for (int x = 0; x < 8; x++) {
    int *green = &greenColumns[x];
    int *red = &redColumns[x];
    *green = 0;
    *red = 0;
    for (int y = 0; y < 8; y++) {
      int cell = display[x][y];
      if (cell & GREEN) {
        *green += 1 << y;
      }
      if (cell & RED) {
        *red += 1 << y;
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

void clearDisplay(int colour) {
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 8; x++)
      display[x][y] = colour;
}

void drawDelay(int milliseconds) {
  long end = millis() + milliseconds;
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

