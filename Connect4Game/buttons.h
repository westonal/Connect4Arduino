#pragma once

#include "pins.h"

#define BTN_DOWN_LEFT (1)
#define BTN_DOWN_RIGHT (2)
#define BTN_DOWN_CENTRE (4)

typedef struct {
   int down;
   int newDown;
   unsigned long downTimeMs;
   int repeatCount;
   unsigned long nextRepeatAllowedAtTime;
} ButtonState;

typedef struct {
   ButtonState left;
   ButtonState right;
   ButtonState centre;
   int mode;
   int repeat1TimeMs;
   int repeatNTimeMs;
} ButtonStates;

ButtonStates *CreateButtonStates();
int readButtons(ButtonStates *states, unsigned long timeMs);

