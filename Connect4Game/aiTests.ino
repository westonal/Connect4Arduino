#include <ArduinoUnit.h>
#include "connect4game.h"

int givenMovesMakeAiChoice(char *moves) {
  Connect4Game *theGame = CreateConnect4Game();
  applyMoves(theGame, moves);
  int aiChoice = aiChooseMove(theGame);
  DisposeGame(theGame);
  return aiChoice;
}

void assertChoice(int expected, char *moves) {
  assertEqual(expected, givenMovesMakeAiChoice(moves));
}

test(ai_win_in_one_move_tests)
{
  assertChoice(3, "001122");
  assertChoice(2, "00113344");
  assertChoice(1, "121212");
}

test(ai_cant_win_but_must_block)
{
  assertChoice(4, "2233555");
}

