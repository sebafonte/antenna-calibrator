char incomingValue[4];
int index;
int currentAngle;
int value=0;
float degreesPerStep = 1.8;
const int stepPinX = 3; 
const int dirPinX = 5; 


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
        
        //Serial.print("Readed: ");
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

void setInitialPosition() {
  currentAngle = 0;
}

int stepsForAngle(float angle) {
  return angle / degreesPerStep;
}

void setRelativePosition(int angle) {
  int steps = stepsForAngle(angle);

  for (int i=0; i< steps; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(stepPinX, HIGH);
    delay(1);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(stepPinX, LOW);
    delay(1);
  }
}

void setAbsolutePosition(int angle) {
  // Calculate angle
  float newAngle = angle - currentAngle;

  // Set direction
  if (newAngle > 0)
    digitalWrite(dirPinX, LOW);
  else {
    newAngle = - newAngle;
    digitalWrite(dirPinX, HIGH);
  }
  
  // Step
  int steps = stepsForAngle(newAngle);

  for (int i=0; i< steps; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(stepPinX, HIGH);
    delay(1);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(stepPinX, LOW);
    delay(1);
  }

  currentAngle = (float) angle;
}

void loop() {
  //setInitialPosition();
  int angle = readAngleCommand();
  
  if (angle > -1) {
    //setRelativePosition(angle);
    setAbsolutePosition(angle);
    delay(1000);
  }
}
