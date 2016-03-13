#include <stdlib.h>
#include "board.h"
#include "display.h"
#include "winchecker.h"

Board *createBoard() {
  Board *n = calloc(1, sizeof(Board));
  return n;
}

int pos(Board *board, int x, int y) {
  int idx = IDX(x, y);
  return ((board->data >> idx) & 1) == 1;
}

void mark(Board *board, int x, int y) {
  if (!board) return;
  int idx = IDX(x, y);
  board->data = board->data | (((uint64_t) 1) << idx);
  board->lastMove.row = y;
  board->lastMove.column = x;
}

void markData(Board *board, uint64_t data) {
  if (board)
    board->data = board->data | data;
}

void resetData(Board *board, uint64_t data) {
  board->data = data;
}

void createCombined(Board* target, Board *b1, Board *b2) {
  target->data = b1->data | b2->data;
}

void draw(Board *board, int colour) {
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH; x++)
      if (pos(board, x, y)) displayPixel(x, 7 - y, colour);
}

void reset(Board *board) {
  board->data = 0;
}

