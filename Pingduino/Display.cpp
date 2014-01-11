#include "Arduino.h"
#include "Display.h"

// Seven Segment Display Handling
byte DigitBytes[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

Display::Display() {
}

void Display::init() {
  _blinkToggle = false;
  _lastBlinkToggle = 0;
}

void Display::refresh(Game game) {
  int p1OnesIndex = game.p1Score() % 10;
  int p1TensIndex = game.p1Score() / 10;
  int p1Ones = DigitBytes[p1OnesIndex];
  int p1Tens = DigitBytes[p1TensIndex];
  
  int p2OnesIndex = game.p2Score() % 10;
  int p2TensIndex = game.p2Score() / 10;
  int p2Ones = DigitBytes[p2OnesIndex];
  int p2Tens = DigitBytes[p2TensIndex];

  p1Tens |= p1IndicatorState(game);
  p2Tens |= p2IndicatorState(game);

  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, p2Ones);
  shiftOut(DATA, CLK, MSBFIRST, p2Tens);
  shiftOut(DATA, CLK, MSBFIRST, p1Ones);
  shiftOut(DATA, CLK, MSBFIRST, p1Tens);
  digitalWrite(LATCH, HIGH);
}

void Display::sleep() {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  digitalWrite(LATCH, HIGH);
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
