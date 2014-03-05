#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

#define DEBOUNCE_TIME 1000

/*
PB0 - button leds
PB1 - display
PB2 - unassigned
PB3 - p1 score
PB4 - p2 score
PB5 - unassigned
*/

#define P1BUTTON  PB3
#define P2BUTTON  PB4
#define BUTTONLED PB0

uint8_t debounce(uint8_t button) {
  if (bit_is_clear(PINB, button)) {
    _delay_us(DEBOUNCE_TIME);
    if (bit_is_clear(PINB, button)) {
      return 1;
    }
  }
  return 0;
}

int main(void) {
  uint8_t p1buttonWasPressed = 0;
  uint8_t p1Score = 0;
  uint8_t p2buttonWasPressed = 0;
  uint8_t p2Score = 0;

  DDRB |= (1 << BUTTONLED);

  PORTB |= (1 << P1BUTTON) | (1 << P2BUTTON);
  PORTB |= (1 << BUTTONLED);

  CLKPR=_BV(CLKPCE);
  CLKPR=0;

  while (1) {
    if (debounce(P1BUTTON)) {
      if (p1buttonWasPressed == 0) {
        p1Score++;
        p1buttonWasPressed = 1;
      }
    } else {
      p1buttonWasPressed = 0;
    }

    if (debounce(P2BUTTON)) {
      if (p2buttonWasPressed == 0) {
        p2Score++;
        p2buttonWasPressed = 1;
      }
    } else {
      p2buttonWasPressed = 0;
    }


    display_score(p1Score, p2Score);
    _delay_ms(10);
  }
  return (0);
}
