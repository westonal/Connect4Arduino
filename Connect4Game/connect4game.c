#include <stdlib.h>
#include "connect4game.h"
#include "display.h"


void debugPositions(Board* red, Board* green) {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 7; y++) {
      if (pos(red, x, y))
      {
//        Serial.println("Red is set ");
//        Serial.print(x);
//        Serial.print(", ");
//        Serial.println(y);
      }
      if (pos(green, x, y))
      {
//        Serial.print("Green is set ");
//        Serial.print(x);
//        Serial.print(", ");
//        Serial.println(y);
      }
    }
}

Connect4Game *CreateConnect4Game(){
  //Serial.println("Created new game");
  Connect4Game *n = calloc(1, sizeof(Connect4Game));  
  n->red = createBoard();
  n->green = createBoard();
  debugPositions(n->red, n->green);
  return n;
}

void Connect4Game_loop(Connect4Game *thiz) {

//  red->mark(0, 0);
//  red->mark(1, 0);
//  green->mark(2, 0);
//  green->mark(2, 1);

  //debugPositions(red, green);

  //red->draw(RED);
  //green->draw(GREEN);

  Connect4Game_processMove(thiz);

  for (int x = 0; x < 8; x++)
    display[x][0] = OFF;
  display[thiz->pos][0] = ORANGE;

  //debugPositions(red, green);

  //display[0][0]=RED;

}

void Connect4Game_processMove(Connect4Game *thiz) {
  if (digitalRead(input_left) == LOW) {
    if ((thiz->mode & BTN_DOWN_LEFT) == 0) {
      //thiz->pos = (thiz->pos + 7)%8;
      //Serial.print("Pos:");
      //Serial.println(thiz->pos);
    }
    thiz->mode |= BTN_DOWN_LEFT;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_LEFT;
  }

  if (digitalRead(input_right) == LOW) {
    if ((thiz->mode & BTN_DOWN_RIGHT) == 0) {
//      Serial.println("Right pressed");
//      Serial.print("Pos:");
//      Serial.println(thiz->pos);
      thiz->pos = (thiz->pos + 1) % 8;
//      Serial.print("Pos:");
//      Serial.println(thiz->pos);
    }
    thiz->mode |= BTN_DOWN_RIGHT;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_RIGHT;
  }

  if (digitalRead(input_centre) == LOW) {
    if ((thiz->mode & BTN_DOWN_CENTRE) == 0) {
      //Serial.println("Centre down");
    }
    thiz->mode |= BTN_DOWN_CENTRE;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_CENTRE;
  }
}

