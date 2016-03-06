#pragma once

#include "Arduino.h"

#include "board.h"

#define BTN_DOWN_LEFT (1)
#define BTN_DOWN_RIGHT (2)
#define BTN_DOWN_CENTRE (4)

#define TURN_GREEN (0)
#define TURN_RED (1)

typedef struct MoveAnimation {
  long startTime;
  int x;
  int targetY;
  int currentY;
  int complete;
  int colour;
  struct MoveAnimation *next;
} MoveAnimation;

typedef struct Connect4Game {
  Board* red;
  Board* green;
  Board* both;
  Board* winBoard;
  int pos;
  int turn;
  int mode;
  long lockedOutUntil;
  int winnerColour;
  MoveAnimation *animations;
} Connect4Game;

Connect4Game *CreateConnect4Game();
void Connect4Game_processMove(Connect4Game *thiz, long timeMs);
void Connect4Game_loop(Connect4Game *thiz, long timeMs);

