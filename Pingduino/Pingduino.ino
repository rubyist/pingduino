#include "Game.h"
#include "Display.h"
#include "VictorySong.h"

#define P1PIN           0 // Interrupts
#define P2PIN           1
#define BUTTONLIGHTPIN  4
#define SPEAKERPIN      9
#define LONGPRESS_TIME  1000
#define INACTIVEMILLIS  1200000 // 20 minutes

// Game Logics
volatile int p1Score = 0;
volatile int p2Score = 0;
volatile int requestReset = 0; // 0 = nobody, 1 = p1, 2 = p2
volatile unsigned long lastActivityTime;

boolean sleeping = false;

Game game;
Display display;
VictorySong victorySong(SPEAKERPIN);

void setup()
{
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
      p1Score++;
    } else {
      // Long press, reset
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

  // Reset the game when requested
  if (game.over() && requestReset) {
    game.restart(requestReset);
    requestReset = 0;
  }
  
  delay(20);
}

/* void showScore(int p1, int p2, int serve, int state) */
/* { */
/*   if (numServes == 2) { */
/*     numServes = 0; */
/*     currentServe = currentServe == 0 ? 1 : 0; */
/*   } */

/*   int p1OnesIndex = p1 % 10; */
/*   int p1TensIndex = p1 / 10; */
/*   int p1Ones = DigitBytes[p1OnesIndex]; */
/*   int p1Tens = DigitBytes[p1TensIndex]; */
  
/*   int p2OnesIndex = p2 % 10; */
/*   int p2TensIndex = p2 / 10; */
/*   int p2Ones = DigitBytes[p2OnesIndex]; */
/*   int p2Tens = DigitBytes[p2TensIndex]; */

/*   if (gameOver) { */
/*     if (currentServe == 0) { */
/*       p1Tens |= 0x80; */
/*     } else { */
/*       p2Tens |= 0x80; */
/*     } */
/*   } */
  
/*   digitalWrite(LATCH, LOW); */
/*   shiftOut(DATA, CLK, MSBFIRST, p2Ones); */
/*   shiftOut(DATA, CLK, MSBFIRST, p2Tens); */
/*   shiftOut(DATA, CLK, MSBFIRST, p1Ones); */
/*   shiftOut(DATA, CLK, MSBFIRST, p1Tens); */
/*   digitalWrite(LATCH, HIGH); */
/* } */

void goToSleep()
{
  /* sleeping = true; */

  /* // Blank score */
  /* digitalWrite(LATCH, LOW); */
  /* shiftOut(DATA, CLK, MSBFIRST, 0x00); */
  /* shiftOut(DATA, CLK, MSBFIRST, 0x00); */
  /* shiftOut(DATA, CLK, MSBFIRST, 0x00); */
  /* shiftOut(DATA, CLK, MSBFIRST, 0x00); */
  /* digitalWrite(LATCH, HIGH); */

  /* // Turn off button lights */
  /* digitalWrite(BUTTONLIGHTPIN, LOW); */
}

 void wakeup()
 {
   if (!sleeping)
     return;
   
   sleeping = false;
   
   //showScore();
   
   digitalWrite(BUTTONLIGHTPIN, HIGH);
 }


void checkForWinner()
{
  /* if ((p1Score >= 11 || p2Score >= 11) && abs(p1Score - p2Score) >= 2) { */

  /*   if (p1Score > p2Score) { */
  /*     p1Win = true; */
  /*   } else { */
  /*     p2Win = true; */
  /*   } */
  /*   victorySong.play(); */
  /*   gameOver = 1; */
  /* } */
}

void resetGame()
{
  /* p1Score = 0; */
  /* p2Score = 0; */
  /* p1Win = false; */
  /* p2Win = false; */
  /* gameOver = 0; */
}



