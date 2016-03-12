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

int fastCheckVertWin(WinChecker *checker, Board *board, Board *resultBoard, int column) {
  int result = 0;
  uint64_t data = getData(board);
  for (int y = 0; y < CONNECT4_HEIGHT - 3; y++) {
    uint64_t mask = checker->vMask;
    mask >>= IDX(column, y);
    if (mask & data == mask) {
      result++;
      for (int i = 0; i < 4; i++)
        mark(resultBoard, column, i);
    }
  }
  return result;
}

void init_wins(WinChecker *checker) {
  p("Creating win checker");
  Board *temp = createBoard();
  checker->vMask = createVertWinMask(temp);
  p("Done win checker");
  free(temp);
}



