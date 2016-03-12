#include "winchecker.h"
#include "debug.h"

void init_wins(WinChecker *checker);

WinChecker *createWinChecker() {
  WinChecker *checker = calloc(1, sizeof(WinChecker));
  init_wins(checker);
  return checker;
}

void growBoardData(WinChecker *checker) {
  int oldSize = checker->boardDataCapacity;
  int newSize = oldSize * 2;
  p("Growing array from %d to %d", oldSize, newSize);
  uint64_t *result = realloc(checker->boardData, checker->boardDataCapacity * sizeof(uint64_t));
  if (result) {
    p("Sucess");
    checker->boardData = result;
    checker->boardDataCapacity = newSize;
  }
  else
  {
    p("Failed");
  }
}

void addBoardData(WinChecker *checker, uint64_t boardData) {
  if (checker->boardDataSize == checker->boardDataCapacity) {
    growBoardData(checker);
  }
  if (checker->boardDataSize < checker->boardDataCapacity)
    checker->boardData[checker->boardDataSize++] = boardData;
}

int createVertWins(WinChecker *checker, Board *temp) {
  int boards = 0;
  for (int x = 0; x < CONNECT4_WIDTH; x++)
    for (int y = 0; y < CONNECT4_HEIGHT - 4; y++) {
      reset(temp);
      for (int c = 0; c < 4; c++)
        mark(temp, x, y + c);
      addBoardData(checker, getData(temp));
      boards++;
    }
  return boards;
}

void init_wins(WinChecker *checker) {
  checker->boardDataCapacity = 1;
  checker->boardData = calloc(checker->boardDataCapacity, sizeof(uint64_t));

  Board *temp = createBoard();
  int vboards = createVertWins(checker, temp);
  p("There were %d Vertical winboards", vboards);
  free(temp);
}

