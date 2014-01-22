#ifndef Display_h
#define Display_h

#include "Game.h"
#include <Adafruit_NeoPixel.h>

class Display {
 public:
  Display();
  void init();
  void refresh(Game game);
  void sleep();

 private:
  bool _blinkToggle;
  unsigned long _lastBlinkToggle;

  void showDigit(int d, int offset);
  int p1IndicatorState(Game game);
  int p2IndicatorState(Game game);
  int blinkState();
};

#endif
