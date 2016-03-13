#include <TimeLib.h>

extern "C" {
#include "connect4game.h"
#include "display.h"
#include "pins.h"
#include "buttons.h"
#include "clock.h"
#include "debug.h"
#include "winchecker.h"
#include "defines.h"
}

int inputs[] = {input_left, input_centre, input_right};

Connect4Game *theGame;
ButtonStates *states;

void setup() {
  Serial.begin(9600);
  while (!Serial); // for the Arduino Leonardo/Micro only

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < 3; i++)
    pinMode(inputs[i], INPUT_PULLUP);

  setTime(1, 2, 0, 1, 1, 2015);

  theGame = CreateConnect4Game();
  states = CreateButtonStates();

  states->repeat1TimeMs = 750;
  states->repeatNTimeMs = 200;

  clearDisplay(OFF);
}

int moveLocation;

int DOWN_LEFT = 1;
int DOWN_RIGHT = 2;
int DOWN_CENTRE = 4;

int mode;

void digitalClockDisplay() {
  int hr = hour();
  int min = minute();
  int sec = second();
  int sx = sec % 5;

  drawDigit(0, 0, hr / 10, sx == 3 ? ORANGE : RED, OFF);
  drawDigit(2, 0, hr % 10, sx == 2 ? ORANGE : GREEN, OFF);
  drawDigit(4, 0, min / 10, sx == 1 ? ORANGE : RED, OFF);
  drawDigit(6, 0, min % 10, sx == 0 ? ORANGE : GREEN, OFF);

  if (sec == 0) {
    for (int x = 0; x < 8; x++)
      for (int y = 5; y < 8; y++)
        displayPixel(x, y, GREEN);
  }

  sec += 5;
  int s1 = sec / 5;
  int s2 = (sec - 30) / 5;
  if (s1 > 6) s1 = 6;
  for (int s = 1; s <= s1; s++) {
    displayPixel(s, 6, GREEN);
  }
  for (int s = 1; s <= s2; s++) {
    displayOrPixel(s, 6, RED);
  }
}

void digitalClockLoop() {
  int mode = readButtons(states, millis());

  if (mode & DOWN_LEFT) {
    moveLocation = (moveLocation + 7) % 8;
    setTime((hour() + 1 % 24), minute(), second(), day(), month(), year());
    digitalClockDisplay();
    drawDelay(250);
  }

  if (mode & DOWN_RIGHT) {
    moveLocation = (moveLocation + 1) % 8;
    setTime(hour(), (minute() + 1) % 60, second(), day(), month(), year());
    digitalClockDisplay();
    drawDelay(250);
  }

  if (mode & DOWN_CENTRE) {
    setTime(hour(), minute(), 0, day(), month(), year());
  }

  digitalClockDisplay();
}

void loop() {
#ifdef RUN_TESTS
  tests();
#endif
  clearDisplay(OFF);

  //digitalClockLoop();

  Connect4Game_loop(theGame, millis(), states);

  //call often
  drawDisplay();
}

