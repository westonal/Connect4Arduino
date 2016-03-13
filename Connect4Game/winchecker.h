#pragma once
#include "Arduino.h"
#include "board.h"

typedef struct {
  uint64_t *boardData;
  int boardDataCapacity;
  int boardDataSize;
  uint64_t vMask;
  uint64_t hMask;
  uint64_t dMaskS;
  uint64_t dMaskBS;
} WinChecker;

WinChecker *getWinChecker();

int fastCheckWin(Board *board, Board *resultBoard);
int fastCheckWinNoMarking(Board *board);

