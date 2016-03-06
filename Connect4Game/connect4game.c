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

int getTurnColour(Connect4Game *thiz) {
  return thiz->turn == TURN_RED ? RED : GREEN;
}

#define PER_STEP_SPEED_MS (100)

void animate(Connect4Game *thiz, long timeMs) {
  MoveAnimation *animation = thiz->animations;
  while (animation) {
    long t = timeMs - animation->startTime;
    int progress = t / PER_STEP_SPEED_MS;
    int y = progress;
    if (y > animation->targetY) {
      y = animation->targetY;
      animation->complete = 1;
    }
    display[animation->x][animation->targetY + 1] = OFF;
    display[animation->x][y + 1] = animation->colour;
    animation = animation->next;
  }
}

void tidyCompleteAnimations(Connect4Game *thiz) {
  MoveAnimation *animation = thiz->animations;
  while (animation && animation->complete) {
    free(animation);
    animation = animation->next;
    thiz->animations = animation;
  }
}

void Connect4Game_loop(Connect4Game *thiz, long timeMs) {
  Connect4Game_processMove(thiz, timeMs);

  for (int x = 0; x < 8; x++)
    display[x][0] = OFF;
  display[thiz->pos][0] = getTurnColour(thiz);

  draw(thiz->red, RED);
  draw(thiz->green, GREEN);

  animate(thiz, timeMs);
  tidyCompleteAnimations(thiz);
}

void MoveAnimation_addAnimation(MoveAnimation *thiz, MoveAnimation *animation) {
  while (thiz->next) {
    thiz = thiz->next;
  }
  thiz->next = animation;
}

void Connect4Game_addAnimation(Connect4Game *thiz, MoveAnimation *animation) {
  if (!thiz->animations) {
    thiz->animations = animation;
  }
  else {
    MoveAnimation_addAnimation(thiz->animations, animation);
  }
}

void Connect4Game_processMove(Connect4Game *thiz, long timeMs) {
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
          MoveAnimation *animation = calloc(1, sizeof(MoveAnimation));
          animation->x = thiz->pos;
          animation->targetY = y;
          animation->colour = getTurnColour(thiz);
          animation->startTime = timeMs;
          Connect4Game_addAnimation(thiz, animation);
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

