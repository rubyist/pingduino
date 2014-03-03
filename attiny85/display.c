#include <avr/io.h>
#include "light_ws2812.h"
#include <string.h>

#define LEDSPERSEG 1
#define LEDSEGMENTS 7
#define DIGITS 4
#define LEDSPERDIGIT LEDSPERSEG * LEDSEGMENTS

#define LRED 0
#define LBLU 100
#define LGRN 0

// 4 digits * 7 segments * 2 LEDs per segment + 2 serve indicators = 58 LEDs
struct cRGB led[DIGITS * LEDSPERDIGIT]; // just one digit for now

//  - A -
//  |   |
//  F   B
//  |   |
//  - G -
//  |   |
//  E   C
//  |   |
//  - D -

const int segments[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void display_digit(int digit, int pos) {
  const int *d = segments[digit];
  int start = pos * LEDSPERDIGIT;
  int end = start + LEDSPERDIGIT;
  int x, c, i;

  for (x = start, c=0; x < end; x += LEDSPERSEG, c++) {
    if (d[c]) {
      for (i = 0; i < LEDSPERSEG; i++) {
        led[x+i].r=LRED; led[x+i].g=LGRN; led[x+i].b=LBLU;
      }
    }
  }
}

void display_score(int p1, int p2) {
  int p1_ones = p1 % 10;
  int p1_tens = p1 / 10;
  int p2_ones = p2 % 10;
  int p2_tens = p2 / 10;

  memset(&led, 0, sizeof led);

  display_digit(p1_tens, 0);
  display_digit(p1_ones, 1);
  display_digit(p2_tens, 2);
  display_digit(p2_ones, 3);

  ws2812_setleds(led, DIGITS*LEDSPERDIGIT);
}
