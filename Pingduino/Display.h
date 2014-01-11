#ifndef Display_h
#define Display_h

#define LATCH           7
#define CLK             6
#define DATA            5

#include "Game.h"

class Display {
 public:
  Display();
  void init();
  void refresh(Game game);
};

#endif
