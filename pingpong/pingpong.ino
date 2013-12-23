#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

// End display setup

#define P1PIN           8
#define P2PIN           9
#define SPEAKERPIN      6
#define LONGPRESS_TIME 25

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

Button p1Button(P1PIN);
Button p2Button(P2PIN);
VictorySong victorySong(SPEAKERPIN);

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Ping Ponger");
  
  Serial.begin(9600);
  showScore();
  
  p1Button.init();
  p2Button.init();
  victorySong.init();
}

void loop()
{
  int p1Event, p2Event;
  
  p1Event = p1Button.handle();
  if (!gameOver && p1Event == BUTTON_SHORT) {
    p1Score++;
    numServes++;
    showScore();
    checkForWinner();
  }
  
  if (gameOver && p1Event == BUTTON_LONG) {
    resetGame();
    currentServe = 0;
    numServes = 0;
    showScore();
  }
  
  p2Event = p2Button.handle();
  if (!gameOver && p2Event == BUTTON_SHORT) {
    p2Score++;
    numServes++;
    showScore();
    checkForWinner();
  }
  
  if (gameOver && p2Event == BUTTON_LONG) {
    resetGame();
    currentServe = 1;
    numServes = 0;
    showScore();
  }
  
  delay(20);
}

void showScore()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player 1");
  lcd.setCursor(13, 0);
  lcd.print(p1Score);
  
  
  lcd.setCursor(0, 1);
  lcd.print("Player 2");
  lcd.setCursor(13, 1);
  lcd.print(p2Score);
  
  if (numServes == 2) {
    numServes = 0;
    currentServe = currentServe == 0 ? 1 : 0;
  }
  
  if (currentServe == 0) {
    lcd.setCursor(15, 0);
  } else {
    lcd.setCursor(15, 1);
  }
  lcd.print("*");
  
  Serial.print("P1: ");
  Serial.print(p1Score);
  Serial.print("  P2: ");
  Serial.println(p2Score);
}

void checkForWinner()
{
  if ((p1Score >= 11 || p2Score >= 11) && abs(p1Score - p2Score) >= 2) {

    Serial.print("*** WINNER ***   ");
    if (p1Score > p2Score) {
      Serial.print("P1");
      lcd.setCursor(10, 0);
      lcd.print("WINNER");
    } else {
      Serial.print("P2");
      lcd.setCursor(10, 1);
      lcd.print("WINNER");
    }
    Serial.println("   *** WINNER ***");
    victorySong.play();
    
    gameOver = 1;
  }
}

void resetGame()
{
  p1Score = 0;
  p2Score = 0;
  gameOver = 0;
}



