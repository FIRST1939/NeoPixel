/*
 * Test microphone levels
 */

#define micPin 0

bool changed = false;
int minValue = 5000;
int maxValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

  int micValue;
  micValue = analogRead(micPin);

  if (micValue > maxValue) {
    maxValue = micValue;
    changed = true;
  }
  
  if (micValue < minValue) {
    minValue = micValue;
    changed = true;
  }
  
  if (changed) {
    Serial.print("Min: ");
    Serial.print(minValue, DEC);
    Serial.print(", Max: ");  
    Serial.print(maxValue, DEC);
    Serial.println();
    changed = false;
  }
}
