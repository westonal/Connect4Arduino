#include "winchecker.h"
#include "debug.h"

void init_wins(WinChecker *checker);

WinChecker *winChecker;

WinChecker *getWinChecker() {
  if (winChecker)
    return winChecker;

  winChecker = calloc(1, sizeof(WinChecker));
  init_wins(winChecker);
  return winChecker;
}

uint64_t createVertWinMask(Board *temp) {
  reset(temp);
  for (int y = 0; y < 4; y++)
    mark(temp, 0, y);
  return getData(temp);
}

uint64_t createHozWinMask(Board *temp) {
  reset(temp);
  for (int x = 0; x < 4; x++)
    mark(temp, x, 0);
  return getData(temp);
}

uint64_t createDiagWinMaskS(Board *temp) {
  reset(temp);
  for (int xy = 0; xy < 4; xy++)
    mark(temp, xy, xy);
  return getData(temp);
}

uint64_t createDiagWinMaskBS(Board *temp) {
  reset(temp);
  for (int xy = 0; xy < 4; xy++)
    mark(temp, xy, 3 - xy);
  return getData(temp);
}

int fastCheckVertWin(Board *board, Board *resultBoard) {
  int row = board->lastMarkedRow;
  if (row < 3)return 0;

  int column = board->lastMarkedColumn;

  WinChecker *checker = getWinChecker();
  int result = 0;
  uint64_t data = getData(board);
  uint64_t mask = checker->vMask;

  mask = mask << IDX(column, row - 3);

  if ((mask & data) == mask) {
    result++;
    for (int i = 0; i < 4; i++)
      mark(resultBoard, column, i + row - 3);
  }

  return result;
}

int fastCheckHozWin(Board *board, Board *resultBoard) {
  int column = board->lastMarkedColumn;
  int row = board->lastMarkedRow;

  WinChecker *checker = getWinChecker();
  int result = 0;
  uint64_t data = getData(board);
  for (int x = column - 3; x <= column; x++) {
    if (x < 0)continue;
    if (x > CONNECT4_WIDTH - 4)continue;

    uint64_t mask = checker->hMask;
    mask = mask << IDX(x, row);

    if ((mask & data) == mask) {
      result++;
      for (int i = 0; i < 4; i++)
        mark(resultBoard, i + x, row);
    }
  }
  return result;
}

int fastCheckDiagWin(Board *board, Board *resultBoard) {
  int column = board->lastMarkedColumn;
  int row = board->lastMarkedRow;
  int result = 0;
  uint64_t data = getData(board);
  WinChecker *checker = getWinChecker();

  for (int xy = -3; xy <= 0; xy++) {
    int testC = column + xy;
    int testR = row + xy;
    if (testC < 0)continue;
    if (testR < 0)continue;
    if (testR > CONNECT4_HEIGHT - 4)continue;
    if (testC > CONNECT4_WIDTH - 4)continue;

    uint64_t mask = checker->dMaskS;
    mask = mask << IDX(testC, testR);
    if ((mask & data) == mask) {
      result++;
      for (int i = 0; i < 4; i++)
        mark(resultBoard, testC + i, testR + i);
    }
  }

  return result;
}

void init_wins(WinChecker *checker) {
  p("Creating win checker");
  Board *temp = createBoard();
  checker->vMask = createVertWinMask(temp);
  checker->hMask = createHozWinMask(temp);
  checker->dMaskS = createDiagWinMaskS(temp);
  checker->dMaskBS = createDiagWinMaskBS(temp);
  printBoard(checker->vMask, "vmask");
  printBoard(checker->hMask, "hmask");
  printBoard(checker->dMaskS, "dmaskS");
  printBoard(checker->dMaskBS, "dmaskBS");
  free(temp);
}



