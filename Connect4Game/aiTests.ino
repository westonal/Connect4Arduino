#ifdef RUN_TESTS

#include <ArduinoUnit.h>
#include "connect4game.h"
#include "defines.h"

int givenMovesMakeAiChoice(char *moves, uint64_t *boardData) {
  Connect4Game *theGame = CreateConnect4Game();
  applyMoves(theGame, moves);
  int aiChoice = aiChooseMove(theGame);
  *boardData = theGame->both->data;
  DisposeGame(theGame);
  return aiChoice;
}

void assertChoice(int expected, char *moves) {
  uint64_t gameData;
  int m = givenMovesMakeAiChoice(moves, &gameData);
  if (m != expected) {
    p("Expected move (%d) missed (%d played)", expected, m);
  }
  assertEqual(expected, m);
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

test(ai_must_prevent_trap)
{
  assertChoice(2, "304");
}

test(ai_first_moves)
{
  assertChoice(3, "");
}

#endif
