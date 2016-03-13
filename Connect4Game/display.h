#pragma once

#include "Arduino.h"
#include "pins.h"

#define OFF 0
#define GREEN 1
#define RED 2
#define ORANGE (GREEN | RED)

void drawDisplay();

void stopDisplay();

void clearDisplay();
void drawDelay(int milliseconds);

void displayPixel(byte x, byte y, byte colour);
void displayOrPixel(byte x, byte y, byte colour);

void drawFrame(uint64_t red, uint64_t green);
void drawBmp(uint64_t red, uint64_t green, byte x, byte y);

void setTranslate(int x, int y);

