#include <Stepper.h>

const int stepsPerRevolution = 48;
Stepper myStepper(stepsPerRevolution * 4, 2, 4, 3, 5);

char incomingValue[4];
int index;
int currentAngle;
int value = 0;
// #NOTE: Calculation should be added, involves step configuration and gears

// Nema17 - 1.8
//float degreesPerStep = 360.0 / 661.0;
float degreesPerStep = 7.5;

// #NOTE: This 15ms delay helps avoid step loosing and decent speed
const int stepDelay = 50;
const int finishedMovingDelay = 200;
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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
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
  int sign;
  
  // Set direction
  if (newAngle > 0)
    sign = -1;
  else {
    sign = 1;
    newAngle = - newAngle;
  }

  int steps;
  if (quadrant(angle) == quadrant(currentAngle)) {
    steps = stepsForAngle(newAngle);
  }
  else {
    float correctedAngle = 360.0 - newAngle;    
    steps = stepsForAngle(correctedAngle);
  }

  for (int i=0; i< steps; i++) {
    myStepper.step(sign);
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
    delay(finishedMovingDelay);
  }

  currentState = getSwitchState();
  if ((prevState != currentState) && (prevState != -1)) {
    float angle = (360.0 / 16.0) * (float) currentState;
    setAbsolutePositionNoTwist(angle);
    delay(finishedMovingDelay);
    Serial.println("Manually: " + String(angle));
  }

  prevState = currentState;
}
