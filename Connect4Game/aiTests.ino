#include <ArduinoUnit.h>
#include "connect4game.h"

test(ai_simple_win)
{
  Connect4Game *theGame = CreateConnect4Game();
  playMove(theGame, 0, 0);
  playMove(theGame, 0, 0);
  playMove(theGame, 1, 0);
  playMove(theGame, 1, 0);
  playMove(theGame, 2, 0);
  playMove(theGame, 2, 0);
  int aiChoice = aiChooseMove(theGame);
  assertEqual(4, aiChoice);
  free(theGame);
}

test(ai_simple_win_2)
{
  Connect4Game *theGame = CreateConnect4Game();
  playMove(theGame, 0, 0);
  playMove(theGame, 0, 0);
  playMove(theGame, 1, 0);
  playMove(theGame, 1, 0);
  playMove(theGame, 3, 0);
  playMove(theGame, 3, 0);
  playMove(theGame, 4, 0);
  playMove(theGame, 4, 0);
  int aiChoice = aiChooseMove(theGame);
  assertEqual(2, aiChoice);
  free(theGame);
}

