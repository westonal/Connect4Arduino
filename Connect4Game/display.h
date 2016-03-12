#pragma once

#include "pins.h"

#define OFF 0
#define GREEN 1
#define RED 2
#define ORANGE (GREEN | RED)

extern int display[8][8];

void drawDisplay();
void syncDisplay();

void stopDisplay();

void clearDisplay(int colour);
void drawDelay(int milliseconds);
