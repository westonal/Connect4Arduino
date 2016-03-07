#include <stdlib.h>
#include "board.h"
#include "display.h"

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))

Board *createBoard() {
  Board *n = calloc(1, sizeof(Board));
  n->width = CONNECT4_WIDTH;
  n->height = CONNECT4_HEIGHT;
  return n;
}

int pos(Board *board, int x, int y) {
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = board->_pos[b];
  return ((block >> a) & 1) == 1;
}

void mark(Board *board, int x, int y) {
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = board->_pos[b];
  block = block | (1 << a);
  board->_pos[b] = block;
}

void unmark(Board *board, int x, int y) {
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = board->_pos[b];
  block = block & ~(1
  << a);
  board->_pos[b] = block;
}

void createCombined(Board* target, Board *b1, Board *b2) {
  for (int i = 0; i < 4; i++)
    target->_pos[i] = b1->_pos[i] | b2->_pos[i];
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
      result += checkDiagWinInSubBoxDirection1(board, resultBoard, x, y) + checkDiagWinInSubBoxDirection2(board, resultBoard, x, y);
  return result;
}

int checkWin(Board *board, Board *resultBoard) {
  return checkHozWin(board, resultBoard) + checkVertWin(board, resultBoard) + checkDiagWin(board, resultBoard);
}

void reset(Board *board) {
  for (int i = 0; i < 4; i++) {
    board->_pos[i] = 0;
  }
}

