
int index;
int currentPosition;
int value=0;

const int stepPinX = 3; 
const int dirPinX = 5; 



void setup() {
  Serial.begin(9600);
  
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
  digitalWrite(dirPinX, HIGH);

  digitalWrite(dirPinX, HIGH);
  
  for (int i=0; i < 661; i++) {
  delay(2);
  digitalWrite(stepPinX, HIGH);
  delay(2);
  digitalWrite(stepPinX, LOW);
  }

  digitalWrite(dirPinX, LOW);
  
  for (int i=0; i < 661; i++) {
  delay(2);
  digitalWrite(stepPinX, HIGH);
  delay(2);
  digitalWrite(stepPinX, LOW);
  }

    
 /* digitalWrite(dirPinX, LOW);
  for (int i=0; i < 800; i++) {
  delay(1);
  digitalWrite(stepPinX, LOW);
  delay(1);
  digitalWrite(stepPinX, HIGH);
  }*/

  delay(5000);
}
