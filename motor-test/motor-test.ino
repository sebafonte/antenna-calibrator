/* MOTOR TEST
 *  
 *  Antenna should give on round in a anticlock wise direction, 
 *  then another round back in clockwise direction to get back
 *  on the beginning.
 */

const int stepPinX = 8; 
const int dirPinX = 7; 
const int STEPSPORVUELTA = 661;

void setup() {
 Serial.begin(9600);
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
  digitalWrite(LED_BUILTIN, LOW);  
  delay(2500);
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (int i=0; i < STEPSPORVUELTA; i++) {
    delay(25);
    digitalWrite(stepPinX, HIGH);
    delay(25);
    digitalWrite(stepPinX, LOW);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(dirPinX, LOW);
  digitalWrite(dirPinX, LOW);
  delay(2500);
  digitalWrite(LED_BUILTIN, HIGH);

  for (int i=0; i < STEPSPORVUELTA; i++) {
    delay(25);
    digitalWrite(stepPinX, HIGH);
    delay(25);
    digitalWrite(stepPinX, LOW);
  }

  // Enganchate aca para apagar / resetear la posicion  
  digitalWrite(LED_BUILTIN, LOW);
  delay(2500);
}
