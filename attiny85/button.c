#include "button.h"
#include <util/delay.h>

#define DEBOUNCE_TIME 1000 // us

#define PRESSTIME      500 // ms
#define LONGPRESSTIME 1000 // ms

enum ButtonState {
  Start,
  Waiting,
  PressCount,
  ReleaseWait,
  LongPressWait
};

/*
A button has:
pin
state
start time
callbacks: press, double press, long press

switch(button_tick(p1Button)) {
  case PRESS:
  case LONGPRESS:
  case DOUBLEPRESS:
}
*/

Button new_button(uint8_t pin) {
  Button b;
  b.pin = pin;
  b.state = Start;

  return b;
}

uint8_t debounce(uint8_t button) {
  if (bit_is_clear(PINB, button)) {
    _delay_us(DEBOUNCE_TIME);
    if (bit_is_clear(PINB, button)) {
      return 1;
    }
  }
  return 0;
}

ButtonPress button_tick(Button b) {
  uint8_t button = debounce(b.pin);
  //
  // switch (b.state) {
  //   case Start:
  //     if (button == 1) {
  //       b.state = Waiting;
  //       // startTime = now;
  //     }
  //     break;
  //
  //   case Waiting:
  //     if (button == 0) {
  //       b.state = PressCount;
  //     } else if ((button == 1) && (now > b.start_time + LONGPRESSTIME)) {
  //       b.state = LongPressWait;
  //     }
  //     break;
  //
  //   case PressCount:
  //     if (now > b.start_time + PRESSTIME) {
  //       b.state = Start;
  //       return SinglePress;
  //     } else if (button == 1) {
  //       b.state = ReleaseWait;
  //     }
  //     break;
  //
  //   case ReleaseWait:
  //     if (button == 0) {
  //       b.state = Start;
  //       return DoublePress;
  //     }
  //     break;
  //
  //   case LongPressWait:
  //     if (button == 0) {
  //       b.state = Start;
  //       return LongPress;
  //     }
  //     break;
  // }
  return NoPress;
}
