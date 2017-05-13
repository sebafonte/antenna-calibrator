
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
  delay(1);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(stepPinX, HIGH);
  delay(1);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(stepPinX, LOW);
}
