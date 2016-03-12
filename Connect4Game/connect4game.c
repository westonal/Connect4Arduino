#include <stdlib.h>
#include "connect4game.h"
#include "display.h"

Connect4Game *CreateConnect4Game() {
  Connect4Game *n = calloc(1, sizeof(Connect4Game));
  n->red = createBoard();
  n->green = createBoard();
  n->both = createBoard();
  n->winBoard = createBoard();
  return n;
}

void DisposeGame(Connect4Game *thiz) {
  free(thiz->red);
  free(thiz->green);
  free(thiz->both);
  free(thiz->winBoard);
  MoveAnimation *current = thiz->animations;
  while (current)
  {
    MoveAnimation *prev = current;
    current = current->next;
    free(prev);
  }
  free(thiz);
}

int getTurnColour(Connect4Game *thiz) {
  return thiz->turn == TURN_RED ? RED : GREEN;
}

#define PER_STEP_SPEED_MS (100)
#define MOVES_MUST_BE_APART_BY_MS (500)

void animate(Connect4Game *thiz, unsigned long timeMs) {
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

void resetGame(Connect4Game *thiz) {
  reset(thiz->red);
  reset(thiz->green);
  reset(thiz->both);
  reset(thiz->winBoard);
  thiz->pos = 0;
  thiz->turn = 0;
  thiz->lockedOutUntil = 0;
  thiz->winnerColour = 0;
}

void Connect4Game_processMove(Connect4Game *thiz, unsigned long timeMs, ButtonStates *states);

void Connect4Game_loop(Connect4Game *thiz, unsigned long timeMs, ButtonStates *states) {
  if (timeMs > thiz->lockedOutUntil && thiz->winnerColour == 0) {
    for (int x = 0; x < 8; x++)
      display[x][0] = OFF;
    display[thiz->pos][0] = getTurnColour(thiz);

    Connect4Game_processMove(thiz, timeMs, states);
  }

  draw(thiz->red, RED);
  draw(thiz->green, GREEN);

  animate(thiz, timeMs);
  tidyCompleteAnimations(thiz);

  if (!thiz->animations) {
    if (thiz->winnerColour) {
      if (timeMs % 600 > 400) {
        draw(thiz->winBoard, ORANGE);
      }
      int i = (timeMs / 100) % 7;
      if (i > 3) i = 6 - i;
      for (int x = i; x < 8 - i; x++)
        display[x][0] = thiz->winnerColour;
      int mode = readButtons(states, timeMs);
      if (mode & BTN_DOWN_CENTRE) {
        resetGame(thiz);
        thiz->lockedOutUntil = timeMs + MOVES_MUST_BE_APART_BY_MS;
      }
    } else {
      //no winner, ai?
      if (thiz->turn == TURN_RED) {
        int m = aiChooseMove(thiz);
        playMove(thiz, m, millis());
      }
    }
  }
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

Board *getCurrentPlayersBoard(Connect4Game *thiz) {
  return thiz->turn == TURN_RED ? thiz->red : thiz->green;
}

Board *getOtherPlayersBoard(Connect4Game *thiz) {
  return thiz->turn == TURN_GREEN ? thiz->red : thiz->green;
}

int getAvailableYPosition(Board *both, int x) {
  for (int y = CONNECT4_HEIGHT - 1; y >= 0; y--)
    if (!pos(both, x, y)) return y;
  return -1;
}

void toggleTurn(Connect4Game * thiz) {
  thiz->turn = 1 - thiz->turn;
}

void playMove(Connect4Game * thiz, int x, unsigned long timeMs) {
  int y = getAvailableYPosition(thiz->both, x);
  if (y != -1) {
    Board *playersBoard = getCurrentPlayersBoard(thiz);
    int turnColour = getTurnColour(thiz);
    mark(playersBoard, x, y);
    MoveAnimation *animation = calloc(1, sizeof(MoveAnimation));
    animation->x = x;
    animation->targetY = y;
    animation->colour = turnColour;
    animation->startTime = timeMs;
    Connect4Game_addAnimation(thiz, animation);
    createCombined(thiz->both, thiz->red, thiz->green);
    if (fastCheckWin(playersBoard, thiz->winBoard)) {
      thiz->winnerColour = turnColour;
    }
    toggleTurn(thiz);
    thiz->lockedOutUntil = timeMs + MOVES_MUST_BE_APART_BY_MS;
  }
}

void Connect4Game_processMove(Connect4Game * thiz, unsigned long timeMs, ButtonStates * states) {
  int mode = readButtons(states, timeMs);

  if (mode & BTN_DOWN_LEFT) {
    thiz->pos = (thiz->pos + (CONNECT4_WIDTH - 1)) % CONNECT4_WIDTH;
  }

  if (mode & BTN_DOWN_RIGHT) {
    thiz->pos = (thiz->pos + 1) % CONNECT4_WIDTH;
  }

  if (mode & BTN_DOWN_CENTRE) {
    playMove(thiz, thiz->pos, timeMs);
  }
}

int aiTestMoveSequence(Connect4Game * thiz, int move1x, int n) {
  Board *playersBoard = getCurrentPlayersBoard(thiz);
  Board *opponentsBoard = getOtherPlayersBoard(thiz);
  Board *both = thiz->both;
  int result = 0;
  int y = getAvailableYPosition(thiz->both, move1x);
  if (y == -1) return 0;

  mark(playersBoard, move1x, y);
  mark(both, move1x, y);
  int win = fastCheckWinNoMarking(playersBoard);
  if (win) {
    result = 1;
  } else if (n > 1) {
    toggleTurn(thiz);
    for (int opponentX = 0; opponentX < CONNECT4_WIDTH; opponentX++) {
      int opponentMoveQuality = aiTestMoveSequence(thiz, opponentX, n - 1);
      if (opponentMoveQuality > 0) {
        result = -1;
      }
    }
    toggleTurn(thiz);
  }
  unmark(both, move1x, y);
  unmark(playersBoard, move1x, y);
  return result;
}

int aiChooseMove(Connect4Game * thiz) {
  int moves[CONNECT4_WIDTH];
  for (int x = 0; x < CONNECT4_WIDTH; x++) {
    moves[x] = aiTestMoveSequence(thiz, x, 2);
  }

  int bestMove = -1000;
  int n = 0;
  for (int x = 0; x < CONNECT4_WIDTH; x++)
    if (moves[x] > bestMove) {
      bestMove = moves[x];
      n = x;
    }
  return n;
}

void applyMoves(Connect4Game * thiz, char *sequence) {
  int len = strlen(sequence);
  for (int i = 0; i < len; i++)
    playMove(thiz, sequence[i] - '0', 0);
}

