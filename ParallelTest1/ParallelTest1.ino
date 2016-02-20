const int signalPin1 =  0;
const int signalPin2 =  1;
const int signalPin3 =  2;
const int ledPin     = 13;

int ledDelay = 0;

bool spin1;
bool spin2;
bool spin3;

void setup() {
Serial.begin(9600);
  pinMode(ledPin,     OUTPUT);      
  pinMode(signalPin1, INPUT);     
  pinMode(signalPin2, INPUT);     
  pinMode(signalPin3, INPUT);     
}

int getDelay(bool pin1, bool pin2, bool pin3) {
  if        (!pin1 && !pin2 && !pin3) {
    return 100;
  } else if (!pin1 && !pin2 &&  pin3) {
    return 200;
  } else if (!pin1 &&  pin2 && !pin3) {
    return 300;
  } else if (!pin1 &&  pin2 &&  pin3) {
    return 400;
  } else if ( pin1 && !pin2 && !pin3) {
    return 500;
  } else if ( pin1 && !pin2 &&  pin3) {
    return 600;
  } else if ( pin1 &&  pin2 && !pin3) {
    return 700;
  } else {
    return 800;    
  }
  return 1000;
}


void loop() {
  spin1 = digitalRead(signalPin1) == HIGH;
  spin2 = digitalRead(signalPin2) == HIGH;
  spin3 = digitalRead(signalPin3) == HIGH;
  Serial.print(spin1 ? "On   " : "Off ");
  Serial.print(spin2 ? "On   " : "Off ");
  Serial.print(spin3 ? "On" : "Off");
  Serial.println();
  
  ledDelay    = getDelay((digitalRead(signalPin1) == HIGH),
                         (digitalRead(signalPin2) == HIGH),
                         (digitalRead(signalPin3) == HIGH));

  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}
