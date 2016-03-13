#pragma once

#include "Arduino.h"
#include "pins.h"

#define OFF 0
#define GREEN 1
#define RED 2
#define ORANGE (GREEN | RED)

extern byte display[8][8];

void drawDisplay();

void stopDisplay();

void clearDisplay(byte colour);
void drawDelay(int milliseconds);

