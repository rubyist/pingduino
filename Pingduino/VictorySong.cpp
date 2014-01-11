#include "Arduino.h"
#include "VictorySong.h"

VictorySong::VictorySong(int p) : pin(p)
{
}

void VictorySong::init()
{  
  pinMode(pin, OUTPUT);
  songLength = 17;
  notes = "efg dab bbbbbaagg";
  tempo = 94;
  _played = false;
}

void VictorySong::play()
{
  _played = true;
  int i, duration;
  int beats[] = {1,2,5,3,2,4,2,5,2,2,2,2,3,3,3,3,2};
  for (i = 0; i < songLength; i++) {
    duration = beats[i] * tempo;
    
    if (notes[i] == ' ') {
      delay(duration);
    } else {
      tone(pin, frequency(notes[i]), duration);
      delay(duration);
    }
    delay(tempo/10);
  }
}

bool VictorySong::played() { return _played; }
void VictorySong::reset() { _played = false; }

int VictorySong::frequency(char note)
{
  int i;
  const int numNotes = 8;
  char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (i = 0; i < numNotes; i++) {
    if (names[i] == note) {
      return(frequencies[i]);
    }
  }
  return 0;
}
