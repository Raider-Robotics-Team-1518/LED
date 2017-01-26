int inputPin3 = 3;
int inputPin4 = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(inputPin3, INPUT);
  pinMode(inputPin4, INPUT);
  digitalWrite(inputPin3, HIGH);
  digitalWrite(inputPin4, HIGH);
  Serial.begin(9600);
}

void loop() {
  // the "1 -" part below converts a 1 into a 0 and 0 into a 1
  int reading3 = 1 - digitalRead(inputPin3);
  int reading4 = 1 - digitalRead(inputPin4);
  if (reading3 == 1 && reading4 == 1) {
    Serial.println("Both switches pressed");
  } else if (reading3 == 1 && reading4 == 0) {
    Serial.println("Switch on pin 3 pressed");
  } else if (reading3 == 0 && reading4 == 1) {
    Serial.println("Switch on pin 4 pressed");
  } else {
    Serial.println("Neither switch pressed");
  }
  delay(1000);
}
