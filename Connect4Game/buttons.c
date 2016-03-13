#include <stdlib.h>
#include "Arduino.h"
#include "buttons.h"

ButtonStates *CreateButtonStates() {
  return calloc(1, sizeof(ButtonStates));
}

int readSingleButton(ButtonState *state, unsigned long timeMs, ButtonStates *states, int flag, int pin) {
  int result = 0;
  state->newDown = 0;
  if (digitalRead(pin) == LOW) {
    state->down = 1;
    int canRepeat = state->nextRepeatAllowedAtTime > 0 && timeMs > state->nextRepeatAllowedAtTime;
    if ((states->mode & flag) == 0 || canRepeat) {
      result = flag;
      state->newDown = 1;
      state->downTimeMs = timeMs;
      state->repeatCount++;
      int repeatTime = state->repeatCount > 1 ? states->repeatNTimeMs : states->repeat1TimeMs;
      if (repeatTime > 0) {
        state->nextRepeatAllowedAtTime = timeMs + repeatTime;
      } else {
        state->nextRepeatAllowedAtTime = 0;
      }
    }
    states->mode |= flag;
  } else {
    state->down = 0;
    state->downTimeMs = 0;
    state->repeatCount = 0;
    state->nextRepeatAllowedAtTime = 0;
    states->mode &= ~flag;
  }
  return result;
}

int readButtons(ButtonStates *states, unsigned long timeMs) {
  int result = 0;
  result |= readSingleButton(&states->left, timeMs, states, BTN_DOWN_LEFT, input_left);
  result |= readSingleButton(&states->right, timeMs, states, BTN_DOWN_RIGHT, input_right);
  result |= readSingleButton(&states->centre, timeMs, states, BTN_DOWN_CENTRE, input_centre);
  return result;
}

