#include <stdlib.h>
#include "board.h"
#include "display.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))

Board *createBoard(){
  Board *n = calloc(1, sizeof(Board));
  n->width = CONNECT4_WIDTH;
  n->height = CONNECT4_HEIGHT;
  return n;
}

int pos(Board *board, int x, int y){
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = board->_pos[b];
  return ((block >> a) & 1) == 1;
}

void mark(Board *board, int x, int y){
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = board->_pos[b];
  block = block | (1 << a);
  board->_pos[b] = block;
}

Board* createCombined(Board*b1, Board*b2){
  Board* combined = createBoard();
  for (int i = 0; i < 4; i++)
    combined->_pos[i] = b1->_pos[i] | b2->_pos[i];
  return combined;
}

void draw(Board *board, int colour){
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH; x++)
      if (pos(board, x, y)) display[x][y + 1] = colour;
}

