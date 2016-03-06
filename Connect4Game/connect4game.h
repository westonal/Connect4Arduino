#pragma once

#include "Arduino.h"

#include "board.h"

#define BTN_DOWN_LEFT (1)
#define BTN_DOWN_RIGHT (2)
#define BTN_DOWN_CENTRE (4)

#define TURN_GREEN (0)
#define TURN_RED (1)

typedef struct Connect4Game {
    Board* red;
    Board* green;
    Board* both;
    int pos;
    int turn;
    int mode;
} Connect4Game;

Connect4Game *CreateConnect4Game();
void Connect4Game_processMove(Connect4Game *thiz);    
void Connect4Game_loop(Connect4Game *thiz);

