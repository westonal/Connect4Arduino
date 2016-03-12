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

int fastCheckVertWin(Board *board, Board *resultBoard) {
  int column = board->lastMarkedColumn;
  WinChecker *checker = getWinChecker();
  int result = 0;
  uint64_t data = getData(board);
  for (int y = 0; y < CONNECT4_HEIGHT - 3; y++) {
    uint64_t mask = checker->vMask;
    mask = mask << IDX(column, y);
    if ((mask & data) == mask) {
      result++;
      for (int i = 0; i < 4; i++)
        mark(resultBoard, column, i + y);
    }
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

void init_wins(WinChecker *checker) {
  p("Creating win checker");
  Board *temp = createBoard();
  checker->vMask = createVertWinMask(temp);
  checker->hMask = createHozWinMask(temp);
  print64(checker->vMask, "vmask");
  print64(checker->hMask, "hmask");
  free(temp);
}



