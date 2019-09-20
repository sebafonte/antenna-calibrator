/* MOTOR TEST
 *  
 *  Antenna should give on round in a anticlock wise direction, 
 *  then another round back in clockwise direction to get back
 *  on the beginning.
 *  
 *  Steps and delay adjusted for m42sp5 by Mitsumi.
 */

const int stepPinX = 7; 
const int dirPinX = 6; 
const int STEPSPORVUELTA = 48;
const int DELAY = 5;

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
  digitalWrite(dirPinX, LOW);
  digitalWrite(LED_BUILTIN, LOW);  
  delay(10);
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (int i=0; i < STEPSPORVUELTA; i++) {
    delay(DELAY);
    digitalWrite(stepPinX, LOW);
    delay(DELAY);
    digitalWrite(stepPinX, HIGH);
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(dirPinX, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, HIGH);

  for (int i=0; i < STEPSPORVUELTA; i++) {
    delay(DELAY);
    digitalWrite(stepPinX, HIGH);
    delay(DELAY);
    digitalWrite(stepPinX, LOW);
  }
}
