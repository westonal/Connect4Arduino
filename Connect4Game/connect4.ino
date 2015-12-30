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
    assertEqual(0, b->pos(x,y));
  delete(b);
}
