char incomingValue[4];
int index;
int currentPosition;
int value=0;

const int stepPinX = 3; 
const int dirPinX = 5; 


void setInitialPosition() {
  
}

void setPosition(int angle) {
  
}

int readCommand() {
  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    incomingValue[index++] = incomingByte;
    if (incomingByte == 0) {
      index = 0;
      return atoi(incomingValue);
    }
  }
  else
    return -1;
}

void setup() {
  Serial.begin(9600);
  
  incomingValue[0] = 0;
  index = 0;
  currentPosition = 0;

  pinMode(stepPinX, OUTPUT); 
  pinMode(dirPinX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(dirPinX, LOW);
  digitalWrite(stepPinX, LOW);
}

void loop() {

  delay(1);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(stepPinX, HIGH);
  delay(1);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(stepPinX, LOW);
  // Read serial command
  
  //digitalWrite(stepPinX, HIGH);
  //digitalWrite(dirPinX, HIGH);
  
/*
  if (angle >= 0) {
    setPosition(angle);
  }
*/
}
