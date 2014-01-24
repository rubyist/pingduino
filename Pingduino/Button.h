#ifndef Buttoon_h_
#define Button_h_

#include "Arduino.h"

extern "C" {
  typedef void (*buttonCallback)();
}

class Button {
 public:
  Button(int pin);
  void tick();
  void setPressCallback(buttonCallback);
  void setDoublePressCallback(buttonCallback);
  void setLongPressCallback(buttonCallback);

 private:
  int _pin;
  int _state;
  unsigned long _startTime;

  buttonCallback _pressCallback;
  buttonCallback _doublePressCallback;
  buttonCallback _longPressCallback;
};

#endif
