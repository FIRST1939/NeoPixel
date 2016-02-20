/* UART Example, any character received on either the real
   serial port, or USB serial (or emulated serial to the
   Arduino Serial Monitor when using non-serial USB types)
   is printed as a message to both ports.

   This example code is in the public domain.
*/

#include <Adafruit_NeoPixel.h>

// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1

const int ledPin = 13;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(104, 17);

void setup() {
	Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
      pixels.setBrightness(255);
    pixels.begin();
}

void loop() {
        int incomingByte;
        
	if (Serial.available() > 0) {
		incomingByte = Serial.read();
		Serial.print("USB received: ");
		Serial.println(incomingByte, DEC);
    all_on(0xFFFFFF);
    delay(500);
    resetRing();
    delay(500);
	}
}

void all_on(uint32_t color) {
  for (int pixel = 0; pixel < 104; pixel++) {
    pixels.setBrightness(255);
    pixels.setPixelColor(pixel, color);
  }
  pixels.show();
}

// Reset all pixels in the ring to black (i.e. off)

void resetRing() {
    for(int pixel = 0; pixel < 104; pixel++) {
        pixels.setPixelColor(pixel, 0x000000);
        pixels.setBrightness(255);
    }
    pixels.show();
}

