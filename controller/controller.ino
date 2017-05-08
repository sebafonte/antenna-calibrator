char incomingValue[4];
int index;
int currentPosition;

void setup() {
  Serial.begin(9600);
  incomingValue[0] = 0;
  index = 0;
  currentPosition = 0;
}

void setInitialPosition() {
  
}

void setPosition(int angle) {
  
}

void readCommand() {
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

void loop() {
  
  // Read serial command


  if (angle >= 0) {
    setPosition(angle);
  }

}
