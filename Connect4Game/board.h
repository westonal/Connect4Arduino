#pragma once
#include "Arduino.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))

typedef struct Board {
  int width;
  int height;
  uint64_t data;
  int lastMarkedColumn;
  int lastMarkedRow;
} Board;

Board *createBoard();

int pos(Board *board, int x, int y);
void mark(Board *board, int x, int y);
void unmark(Board *board, int x, int y);
void createCombined(Board* target, Board *b1, Board *b2);
void draw(Board *board, int colour);
void reset(Board *board);

void markData(Board *board, uint64_t data);


