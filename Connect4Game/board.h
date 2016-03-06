#pragma once

typedef struct Board {
  int width;
  int height;
  unsigned int _pos[4];
} Board;

Board *createBoard();

int pos(Board *board, int x, int y);
void mark(Board *board, int x, int y);
Board* createCombined(Board *b1, Board *b2);
void draw(Board *board, int colour);

