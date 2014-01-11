#ifndef Game_h
#define Game_h

#include "Arduino.h"

class Game {
 public:
  Game();
  void init();
  int p1Score();
  int p2Score();
  int server();
  bool over();
  int winner();
  
  void updateScore(int p1, int p2);
  void restart(int serve);

 protected:
  int _p1Score;
  int _p2Score;
  int _server;
  int _serves;
};

#endif
