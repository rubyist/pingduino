#define P1PIN           2
#define P2PIN           3
#define LONGPRESS_TIME 25

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
  int now_pressed = digitalRead(pin);
  
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



int p1Score = 0;
int p2Score = 0;

Button p1Button(P1PIN);
Button p2Button(P2PIN);

void setup()
{
  pinMode(p1Pin, INPUT);
  pinMode(p2Pin, INPUT);
  Serial.begin(9600);
  showScore();
  
  p1Button.init();
}

void loop()
{
  int p1Event, p2Event;
 
  p1Event = p1Button.handle();
  if (p1Event) {
    p1Score++;
    showScore();
    checkForWinner();
  }
  
  p2Event = p2Button.handle();
  if (p2Event) {
    p2Score++;
    showScore();
    checkForWinner();
  }
   
  delay(100);
}

void showScore()
{
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
    } else {
      Serial.print("P2");
    }
    Serial.println("   *** WINNER ***");
    
    resetGame();
    showScore();
  }
}

void resetGame()
{
  p1Score = 0;
  p2Score = 0;
}



