const int p1Pin = 2;
const int p2Pin = 3;
int p1Score = 0;
int p2Score = 0;

int lastp1State, lastp2State = 1; // HIGH when open
int p1State, p2State = 1;

void setup()
{
  pinMode(p1Pin, INPUT);
  pinMode(p2Pin, INPUT);
  Serial.begin(9600);
  showScore();
}

void loop()
{
  p1State = digitalRead(p1Pin);
  if (p1State != lastp1State) {
    if (p1State == LOW) {
      p1Score++;
      showScore();
    }
  }
  lastp1State = p1State;
  
  p2State = digitalRead(p2Pin);
  if (p2State != lastp2State) {
    if (p2State == LOW) {
      p2Score++;
      showScore();
    }
  }
  lastp2State = p2State;
}

void showScore()
{
  Serial.print("P1: ");
  Serial.print(p1Score);
  Serial.print("  P2: ");
  Serial.println(p2Score);
}
