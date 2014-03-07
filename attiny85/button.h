#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

typedef struct
{
  uint8_t pin;
  uint8_t state;
} Button;

typedef enum {
  NoPress,
  SinglePress,
  LongPress,
  DoublePress
} ButtonPress;

Button new_button(uint8_t pin);
ButtonPress button_tick(Button b);

#endif // BUTTON_H_
