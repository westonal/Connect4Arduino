#include "board.h"

#include <ArduinoUnit.h>

test(board_width_and_height_as_expected)
{
  Board *b = new Board;
  assertEqual(8, b->width());
  assertEqual(7, b->height());
  delete(b);
}

test(is_blank_when_new)
{
  Board *b = new Board;
  for(int y=0; y < b->height(); y++)
  for(int x=0; x < b->width(); x++)  
    assertEqual(0, b->pos(x, y));
  delete(b);
}

test(can_mark_and_read_back_single)
{
  Board *b = new Board;
  b->mark(3, 4);
  assertEqual(0, b->pos(2, 4));
  assertEqual(1, b->pos(3, 4));
  assertEqual(0, b->pos(3, 5));
  delete(b);
}

test(can_mark_two_and_read_back_single)
{
  Board *b = new Board;
  b->mark(3, 4);
  b->mark(4, 5);
  assertEqual(0, b->pos(2, 4));
  assertEqual(1, b->pos(3, 4));
  assertEqual(1, b->pos(4, 5));
  assertEqual(0, b->pos(3, 5));
  delete(b);
}

test(can_mark_each_and_read_back)
{
  Board *b = new Board;
  for(int y=0; y < b->height(); y++)
  for(int x=0; x < b->width(); x++)  
  {
    b->mark(x, y);
    assertEqual(1, b->pos(x, y));
  }
  delete(b);
}


