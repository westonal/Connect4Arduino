#include "Arduino.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    int space;
    uint64_t _pos;
    int space2;
  public:
    Board();
    int width();
    int height();
    int pos(int,int);
    void mark(int,int);
    Board createCombined(const Board* other);
};

Board::Board () {
  _pos = 0LL;
}

#define BITMASK(x,y) (1LL<<((y)*CONNECT4_WIDTH+(x)))

int Board::pos(int x, int y) {
  return (_pos & BITMASK(x,y))!=0LL;
}

int Board::width() {
  return CONNECT4_WIDTH;  
}

int Board::height() {
  return CONNECT4_HEIGHT;  
}

void print64(uint64_t i64){
  Serial.print((int)(i64>>32), BIN);
  Serial.print(" ");
  Serial.println((int)(i64), BIN);
}

void Board::mark(int x, int y) {
  uint64_t toSet = BITMASK(x,y);
  _pos = _pos | toSet;
}

Board Board::createCombined(const Board* other) {
  Board combined;
  print64(_pos);
  print64(other->_pos);
  combined._pos = _pos | (other->_pos);
  print64(combined._pos);
  return combined;
}

