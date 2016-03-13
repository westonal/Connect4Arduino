#ifdef RUN_TESTS
#include <ArduinoUnit.h>

test(ok)
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

void tests(){  
  Test::run();
}

#endif
