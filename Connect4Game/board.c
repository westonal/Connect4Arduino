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

void markData(Board *board, uint64_t data) {
  board->_p = board->_p | data;
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

void reset(Board *board) {
  board->_p = 0;
}

uint64_t getData(Board *board) {
  return board->_p;
}

