#include "Arduino.h"
#include "Button.h"

#define PRESSTIME      100
#define LONGPRESSTIME 1000 

enum ButtonState {
  Start,
  Waiting,
  PressCount,
  ReleaseWait,
  LongPressWait
};

Button::Button(int pin) {
  pinMode(pin, INPUT);
  _pin = pin;

  _state = Start;
}

void Button::setPressCallback(buttonCallback cb) { _pressCallback = cb; }
void Button::setDoublePressCallback(buttonCallback cb) { _doublePressCallback = cb; }
void Button::setLongPressCallback(buttonCallback cb) { _longPressCallback = cb; }

void Button::tick() {
  int button = digitalRead(_pin);
  unsigned long now = millis();

  switch(_state) {
  case Start:
    if (button == HIGH) {
      _state = Waiting;
      _startTime = now;
    }
    break;

  case Waiting:
    if (button == LOW) {
      _state = PressCount;
    } else if ((button == HIGH) && (now > _startTime + 1000)) {
      _state = LongPressWait;
    }
    break;
    
  case PressCount:
    if (now > _startTime + 400) {
      if (_pressCallback) _pressCallback();
      _state = Start;
    } else if (button == HIGH) {
      _state = ReleaseWait;
    }
    break;

  case ReleaseWait:
    if (button == LOW) {
      if (_doublePressCallback) _doublePressCallback();
      _state = Start;
    }
    break;

  case LongPressWait:
    if (button == LOW) {
      if (_longPressCallback) _longPressCallback();
      _state = Start;
    }
    break;
  }
}
