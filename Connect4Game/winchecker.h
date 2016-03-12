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

int fastCheckVertWin(Board *board, Board *resultBoard);
int fastCheckHozWin(Board *board, Board *resultBoard);
int fastCheckDiagWin(Board *board, Board *resultBoard);

