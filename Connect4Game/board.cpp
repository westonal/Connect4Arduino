#include "Arduino.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    uint64_t  _pos;
  public:
    Board();
    int width();
    int height();
    int pos(int,int);
    void mark(int,int);
};

Board::Board () {
  _pos = 0;
}

#define BITMASK(x,y) (1LL<<((y)*CONNECT4_WIDTH+(x)))

int Board::pos(int x, int y) {
  return (_pos & BITMASK(x,y))!=0;
}

int Board::width() {
  return CONNECT4_WIDTH;  
}

int Board::height() {
  return CONNECT4_HEIGHT;  
}

void Board::mark(int x, int y) {
  _pos |= BITMASK(x,y);
}

