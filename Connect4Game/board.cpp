
#define CONNECT4_WIDTH (8)
#define CONNECT4_HEIGHT (7)

class Board {
    int _pos[CONNECT4_WIDTH][CONNECT4_HEIGHT];
  public:
    Board();
    int width();
    int height();
    int pos(int,int);
    void mark(int,int);
};

Board::Board () {
  for(int y=0; y<CONNECT4_HEIGHT; y++)
  for(int x=0; x<CONNECT4_WIDTH; x++)
    _pos[x][y] = 0;
}

int Board::pos(int x, int y) {
  return _pos[x][y];  
}

int Board::width() {
  return CONNECT4_WIDTH;  
}

int Board::height() {
  return CONNECT4_HEIGHT;  
}

void Board::mark(int x, int y) {
  _pos[x][y] = 1;
}

