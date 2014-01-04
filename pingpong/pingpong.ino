#define P1PIN           8
#define P2PIN           9
#define SPEAKERPIN      6
#define LONGPRESS_TIME 25
#define LATCH 4
#define CLK   3
#define DATA  2
#define INACTIVEMILLIS 600000 // 10 minutes

// Seven Segment Display Handling
byte DigitBytes[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

// Button handling
enum { BUTTON_NONE=0, BUTTON_SHORT, BUTTON_LONG};

class Button {
  public:
    Button(int pin, int longpress_time=LONGPRESS_TIME);
    void init();
    int handle();
  
  protected:
    boolean pressed;
    int counter;
    const int pin;
    const int longpress_time;
};

Button::Button(int p, int t) : pin(p), longpress_time(t)
{
}

void Button::init()
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  pressed = false;
  counter = 0;
}

int Button::handle()
{
  int event;
  int now_pressed = !digitalRead(pin);
  
  if (!now_pressed && pressed) {
    if (counter < longpress_time)
      event = BUTTON_SHORT;
    else
      event = BUTTON_LONG;
  } else {
    event = BUTTON_NONE;
  }
  
  if (now_pressed) {
    ++counter;
  } else {
    counter = 0;
  }
    
  pressed = now_pressed;
  return event;
}

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
int p1Score = 0;
int p2Score = 0;
int currentServe = 0; // p1 = 0, p2 = 1
int numServes = 0;
int gameOver = 0;
int victoryBlinkCounter = 0;

unsigned long lastActivityTime;

boolean p1Win = false;
boolean p2Win = false;
boolean blinkToggle = true;

Button p1Button(P1PIN);
Button p2Button(P2PIN);
VictorySong victorySong(SPEAKERPIN);

void setup()
{
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  showScore();
  
  p1Button.init();
  p2Button.init();
  victorySong.init();
  lastActivityTime = millis();
}


void loop()
{
  int p1Event, p2Event;

  Serial.println(millis() - lastActivityTime);
  
  if ((millis() - lastActivityTime) > INACTIVEMILLIS) {
    shutDownLights();
  }
  
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
  
  p1Event = p1Button.handle();
  if (!gameOver && p1Event == BUTTON_SHORT) {
    lastActivityTime = millis();
    p1Score++;
    numServes++;
    showScore();
    checkForWinner();
  }
  
  if (gameOver && p1Event == BUTTON_LONG) {
    lastActivityTime = millis();
    resetGame();
    currentServe = 0;
    numServes = 0;
    showScore();
  }
  
  p2Event = p2Button.handle();
  if (!gameOver && p2Event == BUTTON_SHORT) {
    lastActivityTime = millis();
    p2Score++;
    numServes++;
    showScore();
    checkForWinner();
  }
  
  if (gameOver && p2Event == BUTTON_LONG) {
    lastActivityTime = millis();
    resetGame();
    currentServe = 1;
    numServes = 0;
    showScore();
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

  if (currentServe == 0) {
    showP1Score(p1Score, true);
  } else {
    showP1Score(p1Score, false);
  }
}

void shutDownLights()
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  shiftOut(DATA, CLK, MSBFIRST, 0x00);
  digitalWrite(LATCH, HIGH);
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
}



