#include "Arduino.h"
#include "Display.h"

// Seven Segment Display Handling
byte DigitBytes[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

Display::Display() {
}

void Display::init() {
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

  if (game.server() == 1) {
    p1Tens |= 0x80;
  } else {
    p2Tens |= 0x80;
  }

  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, p2Ones);
  shiftOut(DATA, CLK, MSBFIRST, p2Tens);
  shiftOut(DATA, CLK, MSBFIRST, p1Ones);
  shiftOut(DATA, CLK, MSBFIRST, p1Tens);
  digitalWrite(LATCH, HIGH);
}
