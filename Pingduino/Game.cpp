#include "Arduino.h"
#include "Game.h"

Game::Game() {
}

void Game::init() {
  _p1Score = 0;
  _p2Score = 0;
  _server = 1;
  _serves = 0;
}

int Game::p1Score() { return _p1Score; }
int Game::p2Score() { return _p2Score; }
int Game::server()  { return _server;  }

bool Game::over() {
  return false;
}

void Game::updateScore(int p1, int p2) {
  // The loop will constantly update the score, so we only act if it has changed
  if ((p1 != _p1Score) || (p2 != _p2Score)) {
    _p1Score = p1;
    _p2Score = p2;

    // Handle server change
  }
}

void Game::restart(int serve) {
}
