#include <Adafruit_NeoPixel.h>
#include "WaveUtil.h"
#include "WaveHC.h"

#include "Button.h"
#include "Game.h"
#include "Display.h"
#include "VictorySong.h"

#define P1PIN           2
#define P2PIN           3
#define LONGPRESS_TIME  1000
#define INACTIVEMILLIS  1200000 // 20 minutes

// Game Logics
volatile int p1Score = 0;
volatile int p2Score = 0;
volatile int requestReset = 0; // 0 = nobody, 1 = p1, 2 = p2
volatile unsigned long lastActivityTime;

boolean sleeping = false;

Button p1Button(P1PIN);
Button p2Button(P2PIN);

Game game;
Display display;
VictorySong victorySong;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing");
  
  p1Button.setPressCallback(p1ButtonPressed);
  p1Button.setLongPressCallback(p1ResetRequest);

  p2Button.setPressCallback(p2ButtonPressed);
  p2Button.setLongPressCallback(p2ResetRequest);

  victorySong.init();
  game.init();
  display.init();
  display.refresh(game);
  
  lastActivityTime = millis();
}

void p1ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) {
    return;
  }
  
  Serial.println("Score p1");
  p1Score++;
}

void p1ResetRequest()
{
  Serial.println("Reset p1");
  victorySong.stop();
  requestReset = 1;
}

void p2ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) {
    return;
  }
  
  Serial.println("Score p2");
  p2Score++;
}

void p2ResetRequest()
{
  Serial.println("Reset p2");
  victorySong.stop();
  requestReset = 2;
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

  // Poll the scoring buttons
  p1Button.tick();
  p2Button.tick();
  
  // Update the game
  game.updateScore(p1Score, p2Score);

  // Show the score
  display.refresh(game);

  // Handle game over actions
  if (game.over()) {
    if (!victorySong.played()) {
      Serial.println("Playing victory song");
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
}

 void wakeup()
 {
   if (!sleeping)
     return;
   
   sleeping = false;
 }
