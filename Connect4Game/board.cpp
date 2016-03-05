#include "Arduino.h"
#include "display.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    unsigned int _pos[4];
  public:
    Board();
    int width();
    int height();
    int pos(int, int);
    void mark(int, int);
    Board* createCombined(const Board* other);
    void draw(int colour);
};

Board::Board () {
  Serial.println("Created new board");
}

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))

int Board::pos(int x, int y) {
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = _pos[b];
  return ((block >> a) & 1) == 1;
}

void Board::mark(int x, int y) {
  int idx = IDX(x, y);
  int a = idx % 16;
  int b = idx / 16;
  unsigned int block = _pos[b];
  block = block | (1 << a);
  _pos[b] = block;
}

int Board::width() {
  return CONNECT4_WIDTH;
}

int Board::height() {
  return CONNECT4_HEIGHT;
}

void print64(uint64_t i64) {
  Serial.print((unsigned long)(i64 >> 32), BIN);
  Serial.print(" ");
  Serial.println((unsigned long)(i64), BIN);
}

Board* Board::createCombined(const Board* other) {
  Board* combined = new Board;
  for (int i = 0; i < 4; i++)
    combined->_pos[i] = _pos[i] | other->_pos[i];
  return combined;
}

void Board::draw(int colour) {
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH; x++)
      if (pos(x, y)) display[x][y + 1] = colour;
}

