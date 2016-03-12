#pragma once
#include "Arduino.h"
#include "board.h"

typedef struct {
  uint64_t *boardData;
  int boardDataCapacity;
  int boardDataSize;
  uint64_t vMask;
} WinChecker;

WinChecker *getWinChecker();

int fastCheckVertWin(Board *board, Board *resultBoard, int column);

