#include "display.h"

int drawColumn;

typedef struct {
  uint64_t greenLEDs;
  uint64_t redLEDs;
} Buffer;

byte foregroundBuffer;
Buffer buffers[2] = {0};

unsigned long nextDisplayDrawAllowed;

#define FPS (100)
#define MICROS (1000000LL / (FPS*8))

void displayPixel(byte x, byte y, byte colour) {
  byte idx = y + x * 8;
  Buffer *background = &buffers[1 - foregroundBuffer];
  uint64_t mask = 1LL << idx;
  if (colour & GREEN)
    background->greenLEDs |= mask;
  else
    background->greenLEDs &= ~mask;
  if (colour & RED)
    background->redLEDs |= mask;
  else
    background->redLEDs &= ~mask;
}

void displayOrPixel(byte x, byte y, byte colour) {
  byte idx = y + x * 8;
  Buffer *background = &buffers[1 - foregroundBuffer];
  uint64_t mask = 1LL << idx;
  if (colour & GREEN)
    background->greenLEDs |= mask;
  if (colour & RED)
    background->redLEDs |= mask;
}

void backgroundToForeground() {
  foregroundBuffer = 1 - foregroundBuffer;
}

void drawDisplay() {
  unsigned long timeMicros = micros();

  if (timeMicros < nextDisplayDrawAllowed) {
    return;
  }

  nextDisplayDrawAllowed = timeMicros + MICROS;

  if (drawColumn == 0)
    backgroundToForeground();

  Buffer *foreground = &buffers[foregroundBuffer];

  byte green = (foreground->greenLEDs >> (drawColumn * 8)) & 0xFF;
  byte red = (foreground->redLEDs >> (drawColumn * 8)) & 0xFF;

  //green
  shiftOut(dataPin, clockPin, MSBFIRST, ~green);

  //red
  shiftOut(dataPin, clockPin, LSBFIRST, ~red);

  //column
  shiftOut(dataPin, clockPin, MSBFIRST, 1 << drawColumn);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(latchPin, LOW);

  drawColumn = (drawColumn + 1) % 8;
}

void clearDisplay(byte colour) {
  Buffer *background = &buffers[1 - foregroundBuffer];
  background->greenLEDs = 0;
  background->redLEDs = 0;
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

