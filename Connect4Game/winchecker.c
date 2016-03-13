#include "winchecker.h"

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
  return temp->data;
}

uint64_t createHozWinMask(Board *temp) {
  reset(temp);
  for (int x = 0; x < 4; x++)
    mark(temp, x, 0);
  return temp->data;
}

uint64_t createDiagWinMaskS(Board *temp) {
  reset(temp);
  for (int xy = 0; xy < 4; xy++)
    mark(temp, xy, xy);
  return temp->data;
}

uint64_t createDiagWinMaskBS(Board *temp) {
  reset(temp);
  for (int xy = 0; xy < 4; xy++)
    mark(temp, xy, 3 - xy);
  return temp->data;
}

int fastCheckVertWin(WinChecker *checker, Board *board, Board *resultBoard) {
  int row = board->lastMove.row;
  if (row < 3)return 0;

  int column = board->lastMove.column;

  int result = 0;
  uint64_t data = board->data;
  uint64_t mask = checker->vMask;

  mask = mask << IDX(column, row - 3);

  if ((mask & data) == mask) {
    result++;
    markData(resultBoard, mask);
  }

  return result;
}

int fastCheckHozWin(WinChecker *checker, Board *board, Board *resultBoard) {
  int column = board->lastMove.column;
  int row = board->lastMove.row;

  int result = 0;
  uint64_t data = board->data;
  for (int x = column - 3; x <= column; x++) {
    if (x < 0)continue;
    if (x > CONNECT4_WIDTH - 4)continue;

    uint64_t mask = checker->hMask;
    mask = mask << IDX(x, row);

    if ((mask & data) == mask) {
      result++;
      markData(resultBoard, mask);
    }
  }
  return result;
}

int fastCheckDiagWinS(WinChecker *checker, Board *board, Board *resultBoard) {
  int column = board->lastMove.column;
  int row = board->lastMove.row;
  int result = 0;
  uint64_t data = board->data;

  for (int xy = 0; xy < 4; xy++) {
    int testC = column - xy;
    int testR = row - xy;
    if (testC < 0)continue;
    if (testR < 0)continue;
    if (testR > CONNECT4_HEIGHT - 4)continue;
    if (testC > CONNECT4_WIDTH - 4)continue;

    uint64_t mask = checker->dMaskS;
    mask = mask << IDX(testC, testR);
    if ((mask & data) == mask) {
      result++;
      markData(resultBoard, mask);
    }
  }

  return result;
}

int fastCheckDiagWinBS(WinChecker *checker, Board *board, Board *resultBoard) {
  int column = board->lastMove.column;
  int row = board->lastMove.row;
  int result = 0;
  uint64_t data = board->data;

  for (int xy = 0; xy < 4; xy++) {
    int testC = column - xy;
    int testR = row + xy - 3;
    if (testC < 0)continue;
    if (testR < 0)continue;
    if (testR > CONNECT4_HEIGHT - 4)continue;
    if (testC > CONNECT4_WIDTH - 4)continue;

    uint64_t mask = checker->dMaskBS;
    mask = mask << IDX(testC, testR);
    if ((mask & data) == mask) {
      result++;
      markData(resultBoard, mask);
    }
  }

  return result;
}

int fastCheckWin(Board *board, Board *resultBoard) {
  WinChecker *checker = getWinChecker();

  return fastCheckVertWin(checker, board, resultBoard) +
         fastCheckHozWin(checker, board, resultBoard) +
         fastCheckDiagWinS(checker, board, resultBoard) +
         fastCheckDiagWinBS(checker, board, resultBoard);
}

int fastCheckWinNoMarking(Board *board) {
  WinChecker *checker = getWinChecker();

  return fastCheckVertWin(checker, board, 0) +
         fastCheckHozWin(checker, board, 0) +
         fastCheckDiagWinS(checker, board, 0) +
         fastCheckDiagWinBS(checker, board, 0);
}

void init_wins(WinChecker *checker) {
  Board *temp = createBoard();
  checker->vMask = createVertWinMask(temp);
  checker->hMask = createHozWinMask(temp);
  checker->dMaskS = createDiagWinMaskS(temp);
  checker->dMaskBS = createDiagWinMaskBS(temp);
  free(temp);
}

