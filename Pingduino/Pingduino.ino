#include "Game.h"
#include "Display.h"
#include "WaveUtil.h"
#include "WaveHC.h"

#define P1PIN           0 // Interrupts
#define P2PIN           1
#define BUTTONLIGHTPIN  4
#define LONGPRESS_TIME  1000
#define INACTIVEMILLIS  1200000 // 20 minutes

// Game Logics
volatile int p1Score = 0;
volatile int p2Score = 0;
volatile int requestReset = 0; // 0 = nobody, 1 = p1, 2 = p2
volatile unsigned long lastActivityTime;

boolean sleeping = false;

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


Game game;
Display display;
VictorySong victorySong;

void setup()
{
  Serial.begin(9600);
  
  // Interrupts for scoring buttons
  attachInterrupt(P1PIN, p1ButtonPressed, CHANGE);
  attachInterrupt(P2PIN, p2ButtonPressed, CHANGE);
  
  // Shift Registers for score display
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  // LEDs inside the scoring buttons
  pinMode(BUTTONLIGHTPIN, OUTPUT);
  digitalWrite(BUTTONLIGHTPIN, HIGH);
  
  victorySong.init();
  game.init();
  display.init();
  display.refresh(game);
  
  lastActivityTime = millis();
}

// Intterupt handlers for scoring
volatile int trackP1Button = 0;
volatile unsigned long p1ButtonChangeMillis = 0;
void p1ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) {
    return;
  }
  
  if (trackP1Button == 0) {
    p1ButtonChangeMillis = millis();
    trackP1Button = 1;
    return;
  }

  if (trackP1Button == 1) {
    trackP1Button = 0;
    unsigned long now = millis();
    if ((now - p1ButtonChangeMillis) <= LONGPRESS_TIME) {
      // Short press, score
      Serial.println("Score p1");
      p1Score++;
    } else {
      // Long press, reset
      Serial.println("Reset p1");
      victorySong.stop();
      requestReset = 1;
    }
  }
}

volatile int trackP2Button = 0;
volatile unsigned long p2ButtonChangeMillis = 0;
void p2ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) {
    return;
  }

  if (trackP2Button == 0) {
    p2ButtonChangeMillis = millis();
    trackP2Button = 1;
    return;
  }

  if (trackP2Button == 1) {
    trackP2Button = 0;
    unsigned long now = millis();
    if ((now - p2ButtonChangeMillis) <= LONGPRESS_TIME) {
      // Short press, score
      p2Score++;
    } else {
      // Long press, reset
      victorySong.stop();
      requestReset = 2;
    }
  }
}

void loop()
{
  // The loop will:

  // Check whether it should go to sleep or wake up
  if ((millis() - lastActivityTime) > INACTIVEMILLIS) {
    goToSleep();
  } else {
    wakeup();
  }

  if (sleeping)
    return;

  // Update the game
  game.updateScore(p1Score, p2Score);

  // Show the score
  display.refresh(game);

  // Handle game over actions
  if (game.over()) {
    if (!victorySong.played()) {
      victorySong.play();
    }

    if (requestReset > 0) {
      game.restart(requestReset);
      victorySong.reset();
      p1Score = p2Score = 0;
      requestReset = 0;
    }
  }
  
  delay(20);
}

void goToSleep()
{
  sleeping = true;
  display.sleep();
  digitalWrite(BUTTONLIGHTPIN, LOW);
}

 void wakeup()
 {
   if (!sleeping)
     return;
   
   sleeping = false;
   
   digitalWrite(BUTTONLIGHTPIN, HIGH);
 }
