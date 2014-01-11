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
  void sleep();

 private:
  bool _blinkToggle;
  unsigned long _lastBlinkToggle;

  int p1IndicatorState(Game game);
  int p2IndicatorState(Game game);
  int blinkState();
};

#endif
