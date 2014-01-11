#include "Arduino.h"
#include "Game.h"

Game::Game() {
}

void Game::init() {
  restart(1);
}

int Game::p1Score() { return _p1Score; }
int Game::p2Score() { return _p2Score; }
int Game::server()  { return _server;  }

bool Game::over() {
  return (_p1Score >= 11 || _p2Score >= 11) && abs(_p1Score - _p2Score) >= 2;
}

void Game::updateScore(int p1, int p2) {
  // The loop will constantly update the score, so we only act if it has changed
  if ((p1 != _p1Score) || (p2 != _p2Score)) {
    _p1Score = p1;
    _p2Score = p2;
    _serves++;

    Serial.print("Updating scores ");
    Serial.print(_p1Score);
    Serial.print(" ");
    Serial.print(_p2Score);
    Serial.print("   ");
    Serial.println(over());

    if (_serves == 2) {
      _serves = 0;
      _server = _server == 1 ? 2 : 1;
    }
  }
}

void Game::restart(int serve) {
  _p1Score = 0;
  _p2Score = 0;
  _server = 1;
  _serves = 0;
}
