#include "Arduino.h"

#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    unsigned long _pos1;
    unsigned long _pos2;
  public:
    Board();
    int width();
    int height();
    int pos(int,int);
    void mark(int,int);
    Board* createCombined(const Board* other);
};

Board::Board () {
  _pos1 = 0L;
  _pos2 = 0L;
} 

#define IDX(x,y) ((y)*CONNECT4_WIDTH+(x))
#define BITMASK(x,y) (1LL<<IDX(x,y))

int Board::pos(int x, int y) {
  int idx = IDX(x,y);
  if(idx>=32){
    return ((_pos1>>(idx-32))&1) != 0LL;
  }  
  return ((_pos2>>idx)&1) != 0LL;
}

int Board::width() {
  return CONNECT4_WIDTH;  
}

int Board::height() {
  return CONNECT4_HEIGHT;  
}

void print64(uint64_t i64){
  Serial.print((unsigned long)(i64>>32), BIN);
  Serial.print(" ");
  Serial.println((unsigned long)(i64), BIN);
}

void Board::mark(int x, int y) {
  int idx = IDX(x,y);
  if(idx>=32){
    _pos1 = _pos1 | (1 << (idx-32));
  }  
  _pos2 = _pos2 | (1 << idx);
}

Board* Board::createCombined(const Board* other) {
  Board* combined = new Board;
  combined->_pos1 = _pos1 | other->_pos1;
  combined->_pos2 = _pos2 | other->_pos2;
  return combined;
}

