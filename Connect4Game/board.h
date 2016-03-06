#pragma once

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

typedef struct Board {
  int width;
  int height;
  unsigned int _pos[4];
} Board;

Board *createBoard();

int pos(Board *board, int x, int y);
void mark(Board *board, int x, int y);
void createCombined(Board* target, Board *b1, Board *b2);
void draw(Board *board, int colour);

