#pragma once

#include "Arduino.h"
#include "pins.h"

#define OFF 0
#define GREEN 1
#define RED 2
#define ORANGE (GREEN | RED)

void drawDisplay();

void stopDisplay();

void clearDisplay(byte colour);
void drawDelay(int milliseconds);

void displayPixel(byte x, byte y, byte colour);
void displayOrPixel(byte x, byte y, byte colour);

