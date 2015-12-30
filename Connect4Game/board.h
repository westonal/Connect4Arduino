#include "Arduino.h"
#ifndef Board_h
#define Board_h

class Board {
  public:
    Board();
    int width();
    int height();
    int pos(int,int);
    void mark(int,int);
};

#endif

