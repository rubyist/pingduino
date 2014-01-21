#include "Arduino.h"
#include "VictorySong.h"

VictorySong::VictorySong()
{
}

void VictorySong::init()
{
  _played = false;
  _stop = false;

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  if (!card.init()) {
    Serial.println("Card init failed.");
    _initialized = false;
    return;
  }

  card.partialBlockRead(true);

  uint8_t part;
  for (part = 0; part < 5; part++) {
    if (vol.init(card, part))
      break;
  }

  if (part == 5) {
    Serial.println("No valid FAT partition");
    _initialized = false;
    return;
  }

  _initialized = true;
}

void VictorySong::play()
{
  if (!_initialized)
    return;
      
  _played = true;
  
  // Grab the first wav file and play it
  FatReader file;

  if (!root.openRoot(vol)) {
    Serial.println("Can't open root directory");
    return;
  }

  while (root.readDir(dirBuf) > 0) {
    if (dirBuf.name[0] == '.')
      continue;
    
    if (!file.open(vol, dirBuf)) {
      Serial.println("Failed to open the file");
      return;
    }
    
    if (!file.isDir()) {
      if (wave.create(file)) {
        wave.play();
        while(wave.isplaying) {
          delay(100);
        }
        return;
      }
    }
  }  
}

void VictorySong::stop()
{
  _stop = true;
  wave.stop();
}

bool VictorySong::played() { return _played; }

void VictorySong::reset()
{
  _played = false;
  _stop = false;
}


