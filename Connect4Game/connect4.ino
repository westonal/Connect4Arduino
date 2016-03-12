#include "board.h"

#include <ArduinoUnit.h>

/*

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

test(can_mark_and_unmark_back_single)
{
  Board *b = createBoard();
  mark(b, 2, 4);
  mark(b, 3, 4);
  mark(b, 3, 5);
  unmark(b, 3, 4);
  assertEqual(1, pos(b, 2, 4));
  assertEqual(0, pos(b, 3, 4));
  assertEqual(1, pos(b, 3, 5));
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

test(can_mark_and_unmark_each_and_read_back)
{
  Board *b = createBoard();
  for (int y = 0; y < b->height; y++)
    for (int x = 0; x < b->width; x++)
      mark(b, x, y);
  assertEqual(CONNECT4_WIDTH * CONNECT4_HEIGHT, countOnBoard(b));

  for (int y = 0; y < b->height; y++)
    for (int x = 0; x < b->width; x++)
    {
      unmark(b, x, y);
      assertEqual(0, pos(b, x, y));
    }

  assertEqual(0, countOnBoard(b));
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
  for (int y = 0; y < CONNECT4_HEIGHT; y++) {
    for (int x = 0; x <= (CONNECT4_WIDTH - 4); x++) {
      Board *b = createBoard();
      Board *resultBoard = createBoard();
      assertEqual(0, fastCheckWin(b, resultBoard));
      mark(b, x, y);
      mark(b, x + 1, y);
      mark(b, x + 2, y);
      assertEqual(0, fastCheckWin(b, resultBoard));
      assertIsEmpty(resultBoard);
      mark(b, x + 3, y);
      assertEqual(1, fastCheckWin(b, resultBoard));
      for (int c = x; c < x + 4; c++) {
        assertEqual(1, pos(resultBoard, c, y));
      }
      assertEqual(4, countOnBoard(resultBoard));
      free(b);
      free(resultBoard);
    }
  }
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
  for (int x = 0; x < CONNECT4_WIDTH; x++) {
    for (int y = 0; y <= (CONNECT4_HEIGHT - 4); y++) {
      Board *b = createBoard();
      Board *resultBoard = createBoard();
      assertEqual(0, fastCheckWin(b, resultBoard));
      mark(b, x, y);
      mark(b, x, y + 1);
      mark(b, x, y + 2);
      assertEqual(0, fastCheckWin(b, resultBoard));
      assertIsEmpty(resultBoard);
      mark(b, x, y + 3);
      assertEqual(1, fastCheckWin(b, resultBoard));
      for (int c = y; c < y + 4; c++) {
        assertEqual(1, pos(resultBoard, x, c));
      }
      assertEqual(4, countOnBoard(resultBoard));
      free(b);
      free(resultBoard);
    }
  }
}

test(can_detect_4_diag_1)
{
  for (int x = 0; x <= (CONNECT4_WIDTH - 4); x++) {
    for (int y = 0; y <= (CONNECT4_HEIGHT - 4); y++) {
      Board *b = createBoard();
      Board *resultBoard = createBoard();
      assertEqual(0, fastCheckWin(b, resultBoard));
      mark(b, x, y);
      mark(b, x + 1, y + 1);
      mark(b, x + 2, y + 2);
      assertEqual(0, fastCheckWin(b, resultBoard));
      assertIsEmpty(resultBoard);
      mark(b, x + 3, y + 3);
      assertEqual(1, fastCheckWin(b, resultBoard));
      for (int c = 0; c < 4; c++) {
        assertEqual(1, pos(resultBoard, x + c, y + c));
      }
      assertEqual(4, countOnBoard(resultBoard));
      free(b);
      free(resultBoard);
    }
  }
}

test(does_not_wrap_around_diagonally_1)
{
  Board *b = createBoard();
  Board *resultBoard = createBoard();
  assertEqual(0, fastCheckWin(b, resultBoard));
  for (int xy = 0; xy < 4; xy++) {
    mark(b, xy + 5, xy);
    assertEqual(0, fastCheckWin(b, resultBoard));
  }
  free(b);
  free(resultBoard);
}

test(can_detect_4_diag_2)
{
  for (int x = 0; x <= (CONNECT4_WIDTH - 4); x++) {
    for (int y = 0; y <= (CONNECT4_HEIGHT - 4); y++) {
      Board *b = createBoard();
      Board *resultBoard = createBoard();
      assertEqual(0, fastCheckWin(b, resultBoard));
      mark(b, x + 3, y);
      mark(b, x + 2, y + 1);
      mark(b, x + 1, y + 2);
      assertEqual(0, fastCheckWin(b, resultBoard));
      assertIsEmpty(resultBoard);
      mark(b, x, y + 3);
      assertEqual(1, fastCheckWin(b, resultBoard));
      for (int c = 0; c < 4; c++) {
        assertEqual(1, pos(resultBoard, x + (3 - c), y + c));
      }
      assertEqual(4, countOnBoard(resultBoard));
      free(b);
      free(resultBoard);
    }
  }
}
*/

