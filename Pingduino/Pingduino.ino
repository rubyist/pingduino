#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Garan.h>

#include "Button.h"
#include "Game.h"
#include "Display.h"

#define P1PIN           2
#define P2PIN           3
#define LONGPRESS_TIME  1000
#define INACTIVEMILLIS  1200000 // 20 minutes
#define AUDIOTX         6
#define AUDIORX         8

// Game Logics
int p1Score = 0;
int p2Score = 0;
int requestReset = 0; // 0 = nobody, 1 = p1, 2 = p2
unsigned long lastActivityTime;

boolean sleeping = false;
boolean winSent = false;

Button p1Button(P1PIN);
Button p2Button(P2PIN);

Game game;
Display display;

SoftwareSerial garanSerial(AUDIOTX, AUDIORX);
Garan songPlayer(garanSerial);


void setup()
{
  Serial.begin(9600);
  
  p1Button.setPressCallback(p1ButtonPressed);
  p1Button.setLongPressCallback(p1ResetRequest);
  p1Button.setDoublePressCallback(p1DecrementScore);
  
  p2Button.setPressCallback(p2ButtonPressed);
  p2Button.setLongPressCallback(p2ResetRequest);
  p2Button.setDoublePressCallback(p2DecrementScore);

  game.init();
  display.init();
  display.refresh(game);
  
  lastActivityTime = millis();
  Serial.println("dbg: Initialized");
}

void p1ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) return;
  if (game.over()) return;
  
  Serial.println("score: 1,1");
  p1Score++;
}

void p1ResetRequest()
{
  lastActivityTime = millis();
  if (sleeping) return;

  Serial.println("reset: 1");
  requestReset = 1;
}

void p1DecrementScore() {
  lastActivityTime = millis();
  if (sleeping) return;
  if (game.over()) return;

  if (p1Score > 0) {
    Serial.println("score: 1,-1");
    p1Score--;
  }
}

void p2ButtonPressed()
{
  lastActivityTime = millis();
  if (sleeping) return;
  if (game.over()) return;
  
  Serial.println("score: 2,1");
  p2Score++;
}

void p2ResetRequest()
{
  lastActivityTime = millis();
  if (sleeping) return;

  Serial.println("reset: 2");
  requestReset = 2;
}

void p2DecrementScore() {
  lastActivityTime = millis();
  if (sleeping) return;
  if (game.over()) return;

  if (p2Score > 0) {
    Serial.println("score: 2,-1");
    p2Score--;
  }
}

void loop()
{
  // The loop will:

  // Poll the scoring buttons
  p1Button.tick();
  p2Button.tick();
  
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

  // Send game over event
  if (game.over() && !winSent) {
    songPlayer.singlePlay(1);
    Serial.print("win: ");
    Serial.print(p1Score);
    Serial.print(",");
    Serial.println(p2Score);
    winSent = true;
  }

  // Handle game reset
  if (requestReset > 0) {
    game.restart(requestReset);
    p1Score = p2Score = 0;
    requestReset = 0;
    winSent = false;
    songPlayer.stop();
  }
  
  delay(20);
}

void goToSleep()
{
  if (sleeping) return;
  sleeping = true;
  display.sleep();
  Serial.println("dbg: Going to sleep.");
}

 void wakeup()
 {
   if (!sleeping) return;
   
   sleeping = false;
   Serial.println("dbg: Waking up.");
 }
