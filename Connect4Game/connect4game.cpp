#include "board.h"
#include "display.h"

class Connect4Game {    
  public:
    Connect4Game();
    void loop();
  private:    
    Board* red;
    Board* green;
    int pos;
    int mode;
    void processMove();
};

int BTN_DOWN_LEFT = 1;
int BTN_DOWN_RIGHT = 2;
int BTN_DOWN_CENTRE = 4;

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

Connect4Game::Connect4Game () : pos(0), mode(0), red(new Board), green(new Board) {  
  Serial.println("Created new game");
  debugPositions(red, green);
}

void Connect4Game::loop () {

  red->mark(0,0);
  red->mark(1,0);
  green->mark(2,0);
  green->mark(2,1);
  
  //debugPositions(red, green);
  
  //red->draw(RED);
  //green->draw(GREEN);

  processMove();
  
  for(int x=0;x<8;x++)
    display[x][0] = OFF;
  display[pos][0] = ORANGE;
   
  //debugPositions(red, green);

  //display[0][0]=RED;
 
}

void Connect4Game::processMove(){
 if (digitalRead(input_left)==LOW) {
    if ((mode & BTN_DOWN_LEFT)==0) {
      //pos = (pos + 7)%8;
      Serial.print("Pos:");
      Serial.println(pos);
    }
    mode |= BTN_DOWN_LEFT;
  } else {
    mode = mode & ~BTN_DOWN_LEFT;
  }
  
  if (digitalRead(input_right)==LOW) {
     if ((mode & BTN_DOWN_RIGHT)==0) {
       Serial.println("Right pressed");
       Serial.print("Pos:");
       Serial.println(pos);
       pos = (pos + 1)%8;
       Serial.print("Pos:");
       Serial.println(pos);
     }
     mode |= BTN_DOWN_RIGHT;
  } else {    
    mode = mode & ~BTN_DOWN_RIGHT;
  }

  if (digitalRead(input_centre)==LOW) {
     if ((mode & BTN_DOWN_CENTRE)==0) {
        Serial.println("Centre down");
     }
     mode |= BTN_DOWN_CENTRE;
  } else {    
    mode = mode & ~BTN_DOWN_CENTRE;
  }
}

