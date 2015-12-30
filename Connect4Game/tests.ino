#include <ArduinoUnit.h>

test(ok1) 
{
  int x=3;
  int y=3;
  assertEqual(x,y);
}

void tests(){  
  Test::run();
}

