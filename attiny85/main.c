#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "display.h"
#include "button.h"

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

#define WINLED PB2 // temp

int main(void) {
  uint8_t p1Score = 0;
  uint8_t p2Score = 0;

  Button p1Button = new_button(P1BUTTON);
  Button p2Button = new_button(P2BUTTON);

  DDRB |= (1 << BUTTONLED);
  DDRB |= (1 << WINLED); // temp

  PORTB |= (1 << P1BUTTON) | (1 << P2BUTTON);
  PORTB &= ~(1 << WINLED); // temp

  CLKPR=_BV(CLKPCE);
  CLKPR=0;

  TCCR1 |= (1 << CS10) | (1 << CS11);// | (1 << CS12 ) || (0 << CS13);
  TCNT1 = 0;

  while (1) {
    // Poll buttons for scoring
    switch (button_tick(p1Button)) {
      case SinglePress:
        p1Score++;
        break;
      default:
        break;
    }

    switch (button_tick(p2Button)) {
      case SinglePress:
        p2Score++;
        break;
      default:
        break;
    }


    // Update score display
    display_score(p1Score, p2Score);


    // Check for a game win scenario
    if ((p1Score >= 11 || p2Score >= 11) && abs(p1Score - p2Score) >= 2) {
      // Game is over
      PORTB |= (1 << WINLED); // temp
    }


    // _delay_ms(10);
  }
  return (0);
}
