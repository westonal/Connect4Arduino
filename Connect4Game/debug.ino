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

