#include <ArduinoUnit.h>
#include "connect4game.h"

int givenMovesMakeAiChoice(char *moves) {
  Connect4Game *theGame = CreateConnect4Game();
  applyMoves(theGame, moves);
  int aiChoice = aiChooseMove(theGame);
  DisposeGame(theGame);
  return aiChoice;
}

test(ai_win_in_one_move_tests)
{
  assertEqual(3, givenMovesMakeAiChoice("001122"));
  assertEqual(2, givenMovesMakeAiChoice("00113344"));
}

test(ai_cant_win_but_must_block)
{
  unsigned long startMs = millis();
  assertEqual(4, givenMovesMakeAiChoice("2233555"));
  unsigned long endMs = millis();
  Serial.print("AI took ");
  Serial.print(endMs - startMs);
  Serial.println("ms");
}
