#pragma once

#include "Arduino.h"

#include "board.h"
#include "buttons.h"

#define TURN_GREEN (0)
#define TURN_RED (1)

#define WINFLASH(timeMs) ((timeMs) % 600 > 400)

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
  long lockedOutUntil;
  int winnerColour;
  MoveAnimation *animations;
} Connect4Game;

Connect4Game *CreateConnect4Game();
void DisposeGame(Connect4Game *thiz);

void Connect4Game_loop(Connect4Game *thiz, unsigned long timeMs, ButtonStates *states);

void playMove(Connect4Game *thiz, int x, unsigned long timeMs);
void applyMoves(Connect4Game * thiz, char *sequence);

int aiChooseMove(Connect4Game *game);

