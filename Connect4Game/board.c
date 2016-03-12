#include <stdlib.h>
#include "board.h"
#include "display.h"
#include "winchecker.h"

Board *createBoard() {
  Board *n = calloc(1, sizeof(Board));
  n->width = CONNECT4_WIDTH;
  n->height = CONNECT4_HEIGHT;
  return n;
}

int pos(Board *board, int x, int y) {
  int idx = IDX(x, y);
  return ((board->_p >> idx) & 1) == 1;
}

void mark(Board *board, int x, int y) {
  if (!board) return;
  int idx = IDX(x, y);
  board->_p = board->_p | (((uint64_t) 1) << idx);
  board->lastMarkedColumn = x;
  board->lastMarkedRow = y;
}

void unmark(Board *board, int x, int y) {
  int idx = IDX(x, y);
  board->_p = board->_p & ~(((uint64_t) 1) << idx);
}

void createCombined(Board* target, Board *b1, Board *b2) {
  for (int i = 0; i < 4; i++)
    target->_p = b1->_p | b2->_p;
}

void draw(Board *board, int colour) {
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH; x++)
      if (pos(board, x, y)) display[x][y + 1] = colour;
}

int checkHozWin(Board *board, Board *resultBoard) {
  int result = 0;
  for (int y = 0; y < CONNECT4_HEIGHT; y++) {
    int inARow = 0;
    for (int x = 0; x < CONNECT4_WIDTH; x++) {
      if (pos(board, x, y)) {
        inARow++;
        if (inARow >= 4) {
          result++;
          for (int i = x - inARow + 1; i <= x; i++)
            mark(resultBoard, i, y);
        }
      }
      else {
        inARow = 0;
      }
    }
  }
  return result;
}

int checkVertWin(Board *board, Board *resultBoard) {
  int result = 0;
  for (int x = 0; x < CONNECT4_WIDTH; x++) {
    int inARow = 0;
    for (int y = 0; y < CONNECT4_HEIGHT; y++) {
      if (pos(board, x, y)) {
        inARow++;
        if (inARow >= 4) {
          result++;
          for (int i = y - inARow + 1; i <= y; i++)
            mark(resultBoard, x, i);
        }
      }
      else {
        inARow = 0;
      }
    }
  }
  return result;
}

int checkDiagWinInSubBoxDirection1(Board *board, Board *resultBoard, int xs, int ys) {
  int result = 0;
  for (int i = 0; i < 4; i++) {
    if (!pos(board, xs + i, ys + i))
      return 0;
  }
  for (int i = 0; i < 4; i++) {
    mark(resultBoard,  xs + i, ys + i);
  }
  return 1;
}

int checkDiagWinInSubBoxDirection2(Board *board, Board *resultBoard, int xs, int ys) {
  int result = 0;
  for (int i = 0; i < 4; i++) {
    if (!pos(board, xs + (3 - i), ys + i))
      return 0;
  }
  for (int i = 0; i < 4; i++) {
    mark(resultBoard,  xs + (3 - i), ys + i);
  }
  return 1;
}

int checkDiagWin(Board *board, Board *resultBoard) {
  int result = 0;
  for (int x = 0; x <= (CONNECT4_WIDTH - 4); x++)
    for (int y = 0; y <= (CONNECT4_HEIGHT - 4); y++)
      result += checkDiagWinInSubBoxDirection2(board, resultBoard, x, y);
  return result;
}

int checkWin(Board *board, Board *resultBoard) {
  return checkDiagWin(board, resultBoard);
}

int fastCheckWin(Board *board, Board *resultBoard) {
  return checkWin(board, resultBoard) +
         fastCheckVertWin(board, resultBoard) +
         fastCheckHozWin(board, resultBoard) +
         fastCheckDiagWin(board, resultBoard);
}

void reset(Board *board) {
  board->_p = 0;
}

uint64_t getData(Board *board) {
  return board->_p;
}

