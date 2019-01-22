#define   RL_1            8
#define   RL_2            7
#define   RL_3            6
#define   RL_4            5
#define   RL_5            4
#define   BUTTON_MODE     3
#define   BUTTON_HALT     2
#define   MAX_MODE        6     // Maximum number of animation modes
#define   DEBOUNCE_TIME   110    // Debounce time in milliseconds

volatile uint8_t mode = 0;
volatile bool isButtonPressed = false;
uint32_t counter_start = 0;
uint16_t animationDelay[MAX_MODE] = {1000, 2000, 4000, 6000, 8000, 10000};

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
  Serial.println(mode);
}

void runSequence(void)
{
  for (int i = RL_1; i >= RL_5; i--) {

      if (mode == 0) {
        AllRelaysOff();
        return;
      }

      // Light animation 
      counter_start = millis();
      // Delay loop
      while ((millis() - counter_start) < animationDelay[mode - 1]) {
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

void changeMode_ISR()
{
  static unsigned long previousStateChangeMillis = 0;
  static bool previousPinState = HIGH;

  bool pinState = digitalRead(BUTTON_MODE);
  if (pinState == LOW) { // only falling events
    if ((millis() - previousStateChangeMillis) > DEBOUNCE_TIME) { // debounce
      if (mode < MAX_MODE)
        ++mode;
      else
        mode = 1;
      isButtonPressed = true;
    }
  }
  
  previousStateChangeMillis = millis();
}

void haltCall_ISR()
{
  static unsigned long previousStateChangeMillis = 0;
  static bool previousPinState = HIGH;

  bool pinState = digitalRead(BUTTON_HALT);
  if (pinState == LOW) { // only falling events
    if ((millis() - previousStateChangeMillis) > DEBOUNCE_TIME) { // debounce
      mode = 0;
      isButtonPressed = true;
    }
  }
  
  previousStateChangeMillis = millis();
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
