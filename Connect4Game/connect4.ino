#include "board.h"

#include <ArduinoUnit.h>

//test(board_width_and_height_as_expected)
//{
//  Board b;
//  assertEqual(8, b.width());
//  assertEqual(7, b.height());
//}

test(board_width_and_height_as_expected_new)
{
  Board *b = new Board;
  assertEqual(8, b->width());
  assertEqual(7, b->height());
  delete(b);
}

test(is_blank_when_new)
{
  Board *b = new Board;
  for (int y = 0; y < b->height(); y++)
    for (int x = 0; x < b->width(); x++)
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
  for (int y = 0; y < b->height(); y++)
    for (int x = 0; x < b->width(); x++)
    {
      //if (b->pos(x, y)==1) {
      int temp = (b->pos(x, y));
      if (temp!=0) {
        Serial.print("Failed: ");
        Serial.print(temp);
        Serial.print(" ");
        Serial.print(x);
        Serial.print(", ");
        Serial.println(y);
      }
      assertEqual(0, b->pos(x, y));
      b->mark(x, y);
      assertEqual(1, b->pos(x, y));
    }
  delete(b);
}

test(can_combine_two_boards)
{
  Board *b1 = new Board;
  Board *b2 = new Board;
  b1->mark(3, 4);
  b2->mark(4, 5);

  assertEqual(0, b1->pos(2, 4));
  assertEqual(1, b1->pos(3, 4));
  assertEqual(0, b1->pos(4, 5));
  assertEqual(0, b1->pos(3, 5));

  assertEqual(0, b2->pos(2, 4));
  assertEqual(0, b2->pos(3, 4));
  assertEqual(1, b2->pos(4, 5));
  assertEqual(0, b2->pos(3, 5));

  Board* bc = b1->createCombined(b2);
  assertEqual(0, b1->pos(2, 4));
  assertEqual(1, b1->pos(3, 4));
  assertEqual(0, b1->pos(4, 5));
  assertEqual(0, b1->pos(3, 5));

  assertEqual(0, b2->pos(2, 4));
  assertEqual(0, b2->pos(3, 4));
  assertEqual(1, b2->pos(4, 5));
  assertEqual(0, b2->pos(3, 5));

  assertEqual(0, bc->pos(2, 4));
  assertEqual(1, bc->pos(3, 4));
  assertEqual(1, bc->pos(4, 5));
  assertEqual(0, bc->pos(3, 5));

  delete(b1);
  delete(b2);
  delete(bc);
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


