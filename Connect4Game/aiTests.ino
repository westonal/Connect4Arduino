#include <ArduinoUnit.h>
#include "connect4game.h"

int givenMovesMakeAiChoice(char *moves) {
  Connect4Game *theGame = CreateConnect4Game();
  applyMoves(theGame, moves);
  int aiChoice = aiChooseMove(theGame);
  DisposeGame(theGame);
  return aiChoice;
}

test(ai_simple_win)
{
  assertEqual(3, givenMovesMakeAiChoice("001122"));
}

test(ai_simple_win_2)
{
  assertEqual(2, givenMovesMakeAiChoice("00113344"));
}

