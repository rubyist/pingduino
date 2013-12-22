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
      checkForWinner();
    }
  }
  lastp1State = p1State;
  
  p2State = digitalRead(p2Pin);
  if (p2State != lastp2State) {
    if (p2State == LOW) {
      p2Score++;
      showScore();
      checkForWinner();
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
  gameOver = 0;
}
