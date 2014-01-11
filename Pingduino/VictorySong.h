#ifndef VictorySong_h
#define VictorySong_h

#include "Arduino.h"

// Victory Song
class VictorySong {
  public:
    VictorySong(int pin);
    void init();
    void play();
    bool played();
    void reset();
    void stop();
  
  protected:
    const int pin;
    int songLength;
    char *notes;
    int tempo;
    int frequency(char note);
    bool _played;
    bool _stop;
};

#endif
