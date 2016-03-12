#include <stdarg.h>

void p(char *fmt, ...) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

void print64(uint64_t s4, char *id) {
  char string[65];
  int idx = 63;
  while (idx >= 0) {
    string[idx--] = (s4 & 1) ? '1' : '0';
    s4 >>= 1;
  }
  string[64] = 0;
  p("%s: [%s]", id, string);
}

void printBoard(uint64_t boardData, char *id) {
  char string[8][9];
  int idx = 0;
  while (idx < 64) {
    int x = idx % 8;
    int y = idx / 8;
    string[x][y] = (boardData & 1) ? '1' : '0';
    boardData >>= 1;
    idx++;
  }
  p("%s\n[", id);
  for (int row = 6; row >= 0; row--) {
    string[row][8] = 0;
    p("%d:  %s", row, string[row]);
  }
  p("]", id);
}

