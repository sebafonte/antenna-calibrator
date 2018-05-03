char incomingValue[4];
int index;
int currentAngle;
int value = 0;
// #NOTE: Calculation should be added, involves step configuration and gears
float degreesPerStep = 360.0 / 661.0;
const int stepPinX = 8; 
const int dirPinX = 7; 
// #NOTE: This 15ms delay helps avoid step loosing and decent speed
const int stepDelay = 15;
const char *endMovementConstant = "*";

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
        
        //Serial.print("READED: ");
        //Serial.println(value);
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

void loop() {
  //setInitialPosition();
  int angle = readAngleCommand();
  
  if (angle > -1) {
    setAbsolutePositionNoTwist(angle);
    writeFinishedMoving();
    delay(200);
  }
}
