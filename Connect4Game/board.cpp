#include "Arduino.h"
#include "display.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    unsigned long _pos1;
    unsigned long _pos2;
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
  _pos1 = 0L;
  _pos2 = 0L;
}

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))

int Board::pos(int x, int y) {
  int idx = IDX(x, y);
  if (idx < 32) {
    return ((_pos1 >> idx) & 1) == 1;
  }
  return ((_pos2 >> (idx - 32)) & 1) == 1;
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

void Board::mark(int x, int y) {
  int idx = IDX(x, y);
  if (idx < 32) {
    _pos1 = _pos1 | (1LL << idx);
  } else {
    _pos2 = _pos2 | (1LL << (idx - 32));
  }
}

Board* Board::createCombined(const Board* other) {
  Board* combined = new Board;
  combined->_pos1 = _pos1 | other->_pos1;
  combined->_pos2 = _pos2 | other->_pos2;
  return combined;
}

void Board::draw(int colour) {
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH; x++)
      if (pos(x, y)) display[x][y + 1] = colour;
}

