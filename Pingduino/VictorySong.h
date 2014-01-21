#ifndef VictorySong_h
#define VictorySong_h

#include "WaveUtil.h"
#include "WaveHC.h"

class VictorySong {
  public:
    VictorySong();
    void init();
    void play();
    bool played();
    void reset();
    void stop();
  
  protected:
    volatile bool _played;
    bool _stop;
    bool _initialized;

    SdReader card;
    FatVolume vol;
    FatReader root;
    uint8_t dirLevel;
    dir_t dirBuf;
    WaveHC wave;
};

#endif
