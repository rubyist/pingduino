#define P1PIN           0 // Interrupts
#define P2PIN           1
#define BUTTONLIGHTPIN  4
#define SPEAKERPIN      9
#define LONGPRESS_TIME  1000
#define LATCH           7
#define CLK             6
#define DATA            5
#define INACTIVEMILLIS  1200000 // 20 minutes

// Seven Segment Display Handling
byte DigitBytes[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

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

VictorySong::VictorySong(int p) : pin(p)
{
}

void VictorySong::init()
{  
  pinMode(pin, OUTPUT);
  songLength = 17;
  notes = "efg dab bbbbbaagg";
  tempo = 94;
}

void VictorySong::play()
{
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

// Game Logics
volatile int p1Score = 0;
volatile int p2Score = 0;
volatile int currentServe = 0; // p1 = 0, p2 = 1
volatile int requestReset = 0;
volatile int numServes = 0;
volatile unsigned long lastActivityTime;

int gameOver = 0;
int victoryBlinkCounter = 0;

boolean p1Win = false;
boolean p2Win = false;
boolean blinkToggle = true;
boolean sleeping = false;

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
  
  showScore();

  victorySong.init();
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
      if (gameOver)
        return;
      p1Score++;
      numServes++;
    } else {
      // Long press, reset
      currentServe = 0;
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
      if (gameOver)
        return;
      p2Score++;
      numServes++;
    } else {
      // Long press, reset
      currentServe = 1;
      requestReset = 1;
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

  // Show the score
  showScore();

  // Check for a win state
  if (!gameOver)
    checkForWinner();

  // Handle the victory state
  if (gameOver) {
    victoryBlinkCounter++;

    if (victoryBlinkCounter == 25) {
      if (p1Win) {
        showP1Score(p1Score, blinkToggle);
      } else {
        // Blink p2 button
      }
      victoryBlinkCounter = 0;
      blinkToggle = !blinkToggle;
    }
  }

  // Reset the game when requested
  if (gameOver && requestReset) {
    resetGame();
  }
  
  delay(20);
}

void showP1Score(int score, boolean serve) {
  int onesIndex = score % 10;
  int tensIndex = score / 10;

  int ones = DigitBytes[onesIndex];
  int tens = DigitBytes[tensIndex];

  if (serve) {
    tens |= 0x80;
  }
  
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, ones);
  shiftOut(DATA, CLK, MSBFIRST, tens);
  digitalWrite(LATCH, HIGH);
}


void showScore()
{
  if (numServes == 2) {
    numServes = 0;
    currentServe = currentServe == 0 ? 1 : 0;
  }

  int p1OnesIndex = p1Score % 10;
  int p1TensIndex = p1Score / 10;
  int p1Ones = DigitBytes[p1OnesIndex];
  int p1Tens = DigitBytes[p1TensIndex];
  
  int p2OnesIndex = p2Score % 10;
  int p2TensIndex = p2Score / 10;
  int p2Ones = DigitBytes[p2OnesIndex];
  int p2Tens = DigitBytes[p2TensIndex];

  if (currentServe == 0) {
    p1Tens |= 0x80;
  } else {
    p2Tens |= 0x80;
  }
  
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, p2Ones);
  shiftOut(DATA, CLK, MSBFIRST, p2Tens);
  shiftOut(DATA, CLK, MSBFIRST, p1Ones);
  shiftOut(DATA, CLK, MSBFIRST, p1Tens);
  digitalWrite(LATCH, HIGH);
}

void goToSleep()
{
  sleeping = true;

  // Blank p1 score
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  digitalWrite(LATCH, HIGH);

  // Blank p2 score

  // Turn off button lights
  digitalWrite(BUTTONLIGHTPIN, LOW);
}

 void wakeup()
 {
   if (!sleeping)
     return;
   
   sleeping = false;
   
   showScore();
   
   digitalWrite(BUTTONLIGHTPIN, HIGH);
 }


void checkForWinner()
{
  if ((p1Score >= 11 || p2Score >= 11) && abs(p1Score - p2Score) >= 2) {

    if (p1Score > p2Score) {
      p1Win = true;
    } else {
      p2Win = true;
    }
    victorySong.play();
    gameOver = 1;
  }
}

void resetGame()
{
  p1Score = 0;
  p2Score = 0;
  p1Win = false;
  p2Win = false;
  gameOver = 0;
  numServes = 0;
}



