#include "board.h"
#include "display.h"

class Connect4Game {
    Board* red = new Board;
    Board* green = new Board;
  public:
    Connect4Game();
    void loop();
};

void debugPositions(Board* red, Board* green){
  for(int x=0;x<8;x++)
  for(int y=0;y<7;y++){
  if(red->pos(x,y))
  {
    Serial.println("Red is set ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
  if(green->pos(x,y))
  {
    Serial.print("Green is set ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
  }
  }
}

Connect4Game::Connect4Game () {  
  Serial.println("Created new game");
  debugPositions(red, green);
}

void Connect4Game::loop () {

  red->mark(0,0);
  red->mark(1,0);
  green->mark(2,0);
  green->mark(2,1);
  
  //debugPositions(red, green);
  
  red->draw(RED);
  green->draw(GREEN);

  //debugPositions(red, green);

  //display[0][0]=RED;
 
}

