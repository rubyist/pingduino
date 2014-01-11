#ifndef VictorySong_h
#define VictorySong_h

#include "Arduino.h"

// Victory Song
class VictorySong {
  public:
    VictorySong(int pin);
    void init();
    void play();
  
  protected:
    const int pin;
    int songLength;
    char *notes;
    int tempo;
    int frequency(char note);
};

#endif
