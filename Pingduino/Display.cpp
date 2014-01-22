#include "Arduino.h"
#include "Display.h"
#include <Adafruit_NeoPixel.h>

#define DISPLAYCOLOR 0x000005
#define LEDSPERSEG   2
#define DATAPIN 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(84, DATAPIN, NEO_GRB + NEO_KHZ800);

// unsigned long DigitBytes[] = {
//  0x0003FFFF, // 0
//  0x000001F8, // 1
//  0x001C7E3F, // 2
//  0x001C0FFF, // 3
//  0x001F81F8, // 4
//  0x001F8FC7, // 5
//  0x001FFFC7, // 6
//  0x000001FF, // 7
//  0x001FFFFF, // 8
//  0x001F81FF  // 9
// };

int DigitBytes[] = {
  0x0FFF,
  0x000F,
  0x33CF,
  0x30FF,
  0x3C3C,
  0x3CF3,
  0x3FF0,
  0x003F,
  0x3FFF,
  0x3C3F
};

Display::Display() {
}

void Display::init() {
  strip.begin();

  _blinkToggle = false;
  _lastBlinkToggle = 0;
}

void Display::refresh(Game game) {
  int p1Ones = game.p1Score() % 10;
  int p1Tens = game.p1Score() / 10;
  
  int p2Ones = game.p2Score() % 10;
  int p2Tens = game.p2Score() / 10;

  // Player 1
  showDigit(p1Tens, 0);
  showDigit(p1Ones, 7 * LEDSPERSEG);

  // Player 2
  showDigit(p2Tens, 2 * (7 * LEDSPERSEG));
  showDigit(p2Ones, 3 * (7 * LEDSPERSEG));
}

void Display::showDigit(int d, int offset) {
  int digit = DigitBytes[d];

  for (int i = offset; i < offset + (7 * LEDSPERSEG); i++) {
    if (digit & 1) {
      strip.setPixelColor(i, DISPLAYCOLOR);
    } else {
      strip.setPixelColor(i, 0x000000);
    }
    digit = digit >> 1;
  }
  strip.show();
}

void Display::sleep() {
  for (int i = 0; i < 4 * (7 * LEDSPERSEG); i++) {
    strip.setPixelColor(i, 0x000000);
  }
  strip.show();
}

int Display::p1IndicatorState(Game game) {
  if (!game.over()) {
    return game.server() == 1 ? 0x80 : 0x00;
  }

  if (game.winner() == 1) {
    return blinkState();
  }
  
  return 0x00;
}

int Display::p2IndicatorState(Game game) {
  if (!game.over()) {
    return game.server() == 2 ? 0x80 : 0x00;
  }

  if (game.winner() == 2) {
    return blinkState();
  }

  return 0x00;
}

int Display::blinkState() {
  unsigned long now = millis();
  if ((now - _lastBlinkToggle) > 750) {
    _lastBlinkToggle = now;
    _blinkToggle = !_blinkToggle;
  }

  return _blinkToggle == 0 ? 0x00 : 0x80;
}
