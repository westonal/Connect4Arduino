#pragma once
#include "Arduino.h"
#include "board.h"

typedef struct {
  uint64_t *boardData;
  int boardDataCapacity;
  int boardDataSize;
} WinChecker;

WinChecker *createWinChecker();

