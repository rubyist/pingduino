#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

int main(void) {
  CLKPR=_BV(CLKPCE);
  CLKPR=0;

  int n;

  while (1) {
    for (n = 0; n < 12; n++) {
      display_score(n, n);
      _delay_ms(1000);
    }
  }
  return (0);
}
