#include <stdlib.h>
#include "connect4game.h"
#include "display.h"

Connect4Game *CreateConnect4Game() {
  //Serial.println("Created new game");
  Connect4Game *n = calloc(1, sizeof(Connect4Game));
  n->red = createBoard();
  n->green = createBoard();
  n->both = createBoard();
  return n;
}

void Connect4Game_loop(Connect4Game *thiz) {
  Connect4Game_processMove(thiz);

  for (int x = 0; x < 8; x++)
    display[x][0] = OFF;
  display[thiz->pos][0] = thiz->turn == TURN_RED ? RED : GREEN;

  draw(thiz->red, RED);
  draw(thiz->green, GREEN);
}

void Connect4Game_processMove(Connect4Game *thiz) {
  if (digitalRead(input_left) == LOW) {
    if ((thiz->mode & BTN_DOWN_LEFT) == 0) {
      thiz->pos = (thiz->pos + (CONNECT4_WIDTH - 1)) % CONNECT4_WIDTH;
    }
    thiz->mode |= BTN_DOWN_LEFT;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_LEFT;
  }

  if (digitalRead(input_right) == LOW) {
    if ((thiz->mode & BTN_DOWN_RIGHT) == 0) {
      thiz->pos = (thiz->pos + 1) % CONNECT4_WIDTH;
    }
    thiz->mode |= BTN_DOWN_RIGHT;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_RIGHT;
  }

  if (digitalRead(input_centre) == LOW) {
    if ((thiz->mode & BTN_DOWN_CENTRE) == 0) {
      Board *b = thiz->turn == TURN_RED ? thiz->red : thiz->green;
      for (int y = CONNECT4_HEIGHT - 1; y >= 0; y--) {
        if (!pos(thiz->both, thiz->pos, y)) {
          mark(b, thiz->pos, y);
          thiz->turn = 1 - thiz->turn;
          break;
        }
      }
      createCombined(thiz->both, thiz->red, thiz->green);
    }
    thiz->mode |= BTN_DOWN_CENTRE;
  } else {
    thiz->mode = thiz->mode & ~BTN_DOWN_CENTRE;
  }
}

