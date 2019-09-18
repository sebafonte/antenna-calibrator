char incomingValue[4];
int index;
int currentAngle;
int value = 0;
// #NOTE: Calculation should be added, involves step configuration and gears
float degreesPerStep = 360.0 / 661.0;
const int stepPinX = 6; 
const int dirPinX = 7; 
// #NOTE: This 15ms delay helps avoid step loosing and decent speed
const int stepDelay = 15;
const char *endMovementConstant = "*";
const int pushButton1 = 9;
const int pushButton2 = 10;
const int pushButton3 = 11;
const int pushButton4 = 12;

int prevState = -1;
int currentState = -1;


void setup() {
  Serial.begin(9600);
  incomingValue[0] = 0;
  index = 0;
  currentAngle = 0;

  pinMode(stepPinX, OUTPUT); 
  pinMode(dirPinX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(dirPinX, LOW);
  digitalWrite(stepPinX, LOW);

  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP);
  pinMode(pushButton4, INPUT_PULLUP);
}

void writeFinishedMoving() {
    Serial.println(endMovementConstant);
}

int readAngleCommand() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    //Serial.print("Readed char: ");
    //Serial.println(incomingByte);
   
    if (!isDigit(incomingByte)) {
      int value = -1;
      
      if (index > 0) {
        incomingValue[index++] = '\n';
        value = atoi(incomingValue);
        
        Serial.print("READED: ");
        Serial.println(value);
      }
      index = 0;
      return value;
    }
    else
    {
      incomingValue[index++] = incomingByte;
    }
  }
  
  return -1;
}

// #TODO: use a laser to detect initial position
void setInitialPosition() {
  currentAngle = 0;
}

int stepsForAngle(float angle) {
  return angle / degreesPerStep;
}

int quadrant(float angle) {
  if (angle < 180.0)
    return 0;
  else 
    return 1;
}
  
void setAbsolutePositionNoTwist(int angle) {
  // Calculate angle
  float newAngle = angle - currentAngle;

  // Set direction
  if (newAngle > 0)
    digitalWrite(dirPinX, LOW);
  else {
    newAngle = - newAngle;
    digitalWrite(dirPinX, HIGH);
  }

  int steps;
  if (quadrant(angle) == quadrant(currentAngle)) {
    steps = stepsForAngle(newAngle);
  }
  else {
    float correctedAngle = 360.0 - newAngle;
    
    if (digitalRead(dirPinX) == LOW)
      digitalWrite(dirPinX, HIGH);
    else
      digitalWrite(dirPinX, LOW);
    
    steps = stepsForAngle(correctedAngle);
  }

  for (int i=0; i< steps; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(stepPinX, HIGH);
    delay(stepDelay);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(stepPinX, LOW);
    delay(stepDelay);
  }

  currentAngle = (float) angle;
}

int getSwitchState() {
  int buttonState1 = digitalRead(pushButton1);
  int buttonState2 = digitalRead(pushButton2);
  int buttonState3 = digitalRead(pushButton3);
  int buttonState4 = digitalRead(pushButton4);
  return (buttonState1 * 8) + (buttonState2 * 4) + (buttonState3 * 2) + buttonState4;
}

void loop() {
  //setInitialPosition();
  int angle = readAngleCommand();

  if (angle > -1) {
    setAbsolutePositionNoTwist(angle);
    writeFinishedMoving();
    delay(200);
  }

  currentState = getSwitchState();
  if ((prevState != currentState) && (prevState != -1)) {
    float angle = (360.0 / 16.0) * (float) currentState;
    setAbsolutePositionNoTwist(angle);
  }

  prevState = currentState;
}
