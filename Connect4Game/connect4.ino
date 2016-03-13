#ifdef RUN_TESTS

#include "board.h"

#include <ArduinoUnit.h>

#define MOVE(x,y) (((x)<<4)|y)

void assertWin(Board *b, Board *resultBoard, int expected) {
  int win = fastCheckWin(b, resultBoard);
  if (expected != win) {
    p("Test failed expected %s", win ? "win" : "loss");
    printBoard(b->data, "Player board");
  }
  assertEqual(expected, win);
}

void playMoveAssertNotWin(Board *b, Board *resultBoard, int m) {
  int x = m >> 4;
  int y = m & 0xF;
  mark(b, x, y);
  assertWin(b, resultBoard, 0);
}

void playMoveAssertWin(Board *b, Board *resultBoard, int m) {
  int x = m >> 4;
  int y = m & 0xF;
  mark(b, x, y);
  assertWin(b, resultBoard, 1);
}

void assertMoveSet(Board *resultBoard, int m) {
  int x = m >> 4;
  int y = m & 0xF;
  assertEqual(1, pos(resultBoard, x, y));
}

void assertLastMoveWin(int move1, int move2, int move3, int move4) {
  Board *b = createBoard();
  Board *resultBoard = createBoard();
  playMoveAssertNotWin(b, resultBoard, move1);
  playMoveAssertNotWin(b, resultBoard, move2);
  playMoveAssertNotWin(b, resultBoard, move3);
  playMoveAssertWin(b, resultBoard, move4);
  assertMoveSet(resultBoard, move1);
  assertMoveSet(resultBoard, move2);
  assertMoveSet(resultBoard, move3);
  assertMoveSet(resultBoard, move4);
  free(resultBoard);
  free(b);
}

void assertNoWin(int move1, int move2, int move3, int move4) {
  Board *b = createBoard();
  Board *resultBoard = createBoard();
  playMoveAssertNotWin(b, resultBoard, move1);
  playMoveAssertNotWin(b, resultBoard, move2);
  playMoveAssertNotWin(b, resultBoard, move3);
  playMoveAssertNotWin(b, resultBoard, move4);
  free(resultBoard);
  free(b);
}

void assertLastMoveWinAnyOrder(int move1, int move2, int move3, int move4) {
  assertLastMoveWin(move1, move2, move3, move4);
  assertLastMoveWin(move4, move3, move2, move1);
  assertLastMoveWin(move1, move3, move2, move4);
  assertLastMoveWin(move4, move3, move2, move1);
  assertLastMoveWin(move4, move3, move1, move2);
  assertLastMoveWin(move1, move2, move4, move3);
}

test(board_width_and_height_as_expected)
{
  Board *b = createBoard();
  assertEqual(8, b->width);
  assertEqual(7, b->height);
  free(b);
}

int countOnBoard(Board *b) {
  int result = 0;
  for (int y = 0; y < b->height; y++)
    for (int x = 0; x < b->width; x++)
      if (pos(b, x, y))
        result++;
  return result;
}

void assertIsEmpty(Board *b) {
  assertEqual(0, countOnBoard(b));
}

test(is_blank_when_new)
{
  Board *b = createBoard();
  assertIsEmpty(b);
  free(b);
}

test(can_mark_and_read_back_single)
{
  Board *b = createBoard();
  mark(b, 3, 4);
  assertEqual(0, pos(b, 2, 4));
  assertEqual(1, pos(b, 3, 4));
  assertEqual(0, pos(b, 3, 5));
  free(b);
}

test(can_mark_two_and_read_back_single)
{
  Board *b = createBoard();
  mark(b, 3, 4);
  mark(b, 4, 5);
  assertEqual(0, pos(b, 2, 4));
  assertEqual(1, pos(b, 3, 4));
  assertEqual(1, pos(b, 4, 5));
  assertEqual(0, pos(b, 3, 5));
  free(b);
}

test(can_mark_each_and_read_back)
{
  Board *b = createBoard();
  for (int y = 0; y < b->height; y++)
    for (int x = 0; x < b->width; x++)
    {
      int temp = pos(b, x, y);
      assertEqual(0, pos(b, x, y));
      mark(b, x, y);
      assertEqual(1, pos(b, x, y));
    }
  free(b);
}

test(can_combine_two_boards)
{
  Board *b1 = createBoard();
  Board *b2 = createBoard();
  mark(b1, 3, 4);
  mark(b2, 4, 5);

  assertEqual(0, pos(b1, 2, 4));
  assertEqual(1, pos(b1, 3, 4));
  assertEqual(0, pos(b1, 4, 5));
  assertEqual(0, pos(b1, 3, 5));

  assertEqual(0, pos(b2, 2, 4));
  assertEqual(0, pos(b2, 3, 4));
  assertEqual(1, pos(b2, 4, 5));
  assertEqual(0, pos(b2, 3, 5));

  Board* bc = createBoard();
  createCombined(bc, b1, b2);
  assertEqual(0, pos(b1, 2, 4));
  assertEqual(1, pos(b1, 3, 4));
  assertEqual(0, pos(b1, 4, 5));
  assertEqual(0, pos(b1, 3, 5));

  assertEqual(0, pos(b2, 2, 4));
  assertEqual(0, pos(b2, 3, 4));
  assertEqual(1, pos(b2, 4, 5));
  assertEqual(0, pos(b2, 3, 5));

  assertEqual(0, pos(bc, 2, 4));
  assertEqual(1, pos(bc, 3, 4));
  assertEqual(1, pos(bc, 4, 5));
  assertEqual(0, pos(bc, 3, 5));

  free(b1);
  free(b2);
  free(bc);
}

test(sixty_four_bit_operations)
{
  uint64_t a = 2LL;
  uint64_t b = 4LL;
  assertEqual(6, (int)(a | b));
  uint64_t a2 = 1LL << 62;
  uint64_t b2 = a2 >> 62;
  assertEqual(1, (int)(b2));
}

test(can_detect_4_horizontally)
{
  for (int y = 0; y < CONNECT4_HEIGHT; y++)
    for (int x = 0; x < CONNECT4_WIDTH - 3; x++)
      assertLastMoveWinAnyOrder(MOVE(x, y),
                                MOVE(x + 1, y),
                                MOVE(x + 2, y),
                                MOVE(x + 3, y));
}

test(wont_wrap_around_horizontal_wins)
{
  Board *b = createBoard();
  Board *resultBoard = createBoard();

  mark(b, 0, 0);
  mark(b, 1, 0);
  mark(b, 2, 0);
  assertEqual(0, fastCheckWin(b, resultBoard));

  mark(b, 0, 1);
  mark(b, 1, 1);
  mark(b, 2, 1);
  assertEqual(0, fastCheckWin(b, resultBoard));

  mark(b, 7, 0);
  assertEqual(0, fastCheckWin(b, resultBoard));
  mark(b, 7, 1);
  assertEqual(0, fastCheckWin(b, resultBoard));
  mark(b, 7, 2);
  assertEqual(0, fastCheckWin(b, resultBoard));

  free(b);
  free(resultBoard);
}

test(can_detect_4_vertically)
{
  for (int x = 0; x < CONNECT4_WIDTH; x++)
    for (int y = 0; y < CONNECT4_HEIGHT - 3; y++)
      assertLastMoveWin(MOVE(x, y),
                        MOVE(x, y + 1),
                        MOVE(x, y + 2),
                        MOVE(x, y + 3));
}

test(can_detect_4_diag_1)
{
  for (int x = 0; x < CONNECT4_WIDTH - 3; x++)
    for (int y = 0; y < CONNECT4_HEIGHT - 3; y++)
      assertLastMoveWinAnyOrder(MOVE(x, y),
                                MOVE(x + 1, y + 1),
                                MOVE(x + 2, y + 2),
                                MOVE(x + 3, y + 3));
}

test(can_detect_4_diag_2)
{
  for (int x = 0; x < CONNECT4_WIDTH - 3; x++)
    for (int y = 0; y < CONNECT4_HEIGHT - 3; y++)
      assertLastMoveWinAnyOrder(MOVE(x + 3, y),
                                MOVE(x + 2, y + 1),
                                MOVE(x + 1, y + 2),
                                MOVE(x, y + 3));
}

test(does_not_wrap_around_diagonally_1)
{
  assertNoWin(MOVE(5, 0),
              MOVE(6, 1),
              MOVE(7, 2),
              MOVE(0, 3));
}

test(does_not_wrap_around_diagonally_2)
{
  assertNoWin(MOVE(5, 3),
              MOVE(6, 2),
              MOVE(7, 1),
              MOVE(0, 0));
}

#endif

