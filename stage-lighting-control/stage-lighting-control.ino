#define   RL_1          8
#define   RL_2          7
#define   RL_3          6
#define   RL_4          5
#define   RL_5          4
#define   BUTTON_MODE   3
#define   BUTTON_HALT   2
#define   MAX_MODE      6   // Maximum number of animation modes

volatile byte mode = 0;
volatile bool isButtonPressed = false;

void setup()
{
  //DEBUG
  Serial.begin(9600);
  // Outputs
  pinMode(RL_1, OUTPUT);
  pinMode(RL_2, OUTPUT);
  pinMode(RL_3, OUTPUT);
  pinMode(RL_4, OUTPUT);
  pinMode(RL_5, OUTPUT);
  AllRelaysOff();
  // Inputs
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_HALT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MODE), changeMode_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_HALT), haltCall_ISR, FALLING);
}

void loop()
{
  runSequence();
  //Serial.println(mode);
}

void runSequence(void)
{
  int animationDelay;
  int counter_start;
  
  switch(mode) {
    case 0:
      AllRelaysOff();
      return;
    case 1:
      animationDelay = 10000;
      break;
    case 2:
      animationDelay = 8000;
      break;
    case 3:
      animationDelay = 6000;
      break;
    case 4:
      animationDelay = 4000;
      break;
    case 5:
      animationDelay = 2000;
      break;
    case 6:
      animationDelay = 1000;
      break;
    default:
      AllRelaysOff();
      return;
  }
  // Assure button state begins not pressed
  isButtonPressed = false;
  
  for (int i = RL_1; i >= RL_5; i--) {

      // Light animation 
      counter_start = millis();
        
      while ((millis() - counter_start) < animationDelay) {
          setRelayOn(i);
          if (isButtonPressed) {
              AllRelaysOff();
              isButtonPressed = false;
              return;
          } 
      }
      setRelayOff(i);
  }
}

void changeMode_ISR(void)
{
  AllRelaysOff();
  
  if (mode < MAX_MODE)
    ++mode;
  else
    mode = 0;

  isButtonPressed = true;
}

void haltCall_ISR(void)
{
  AllRelaysOff();
  mode = 0;
  isButtonPressed = true;
}

void AllRelaysOff(void)
{
  for (int i = RL_1; i >= RL_5; i--) {
    setRelayOff(i);
  }
}

void AllRelaysOn(void)
{
  for (int i = RL_1; i >= RL_5; i--) {
    setRelayOn(i);
  }
}

void setRelayOn(int relay)
{
  if (relay <= RL_1 && relay >= RL_5)
    digitalWrite(relay, LOW);
}

void setRelayOff(int relay)
{
  if (relay <= RL_1 && relay >= RL_5)
    digitalWrite(relay, HIGH);
}
