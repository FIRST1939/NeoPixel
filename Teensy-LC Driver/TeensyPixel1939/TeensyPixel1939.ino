
/*
 * TeensyPixel1939
 * 
 * Firmware for a Teensy-LC to drive NeoPixels under the direction of
 * a Raspberry Pi communicating via serial using a JSON-formatted 
 * protocol.
 * 
 * Library Documentation:
 *      - https://github.com/bblanchon/ArduinoJson/wiki
 *      - https://github.com/adafruit/Adafruit_NeoPixel
 */

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#define NEOPIN     17
#define NUMPIXELS  12
#define BRIGHTNESS 20
#define HWSERIAL   Serial1

// Define color patterns

#define COLOR_BLACK  0x000000
#define COLOR_RED    0xFF0000
#define COLOR_ORANGE 0xFF7F00
#define COLOR_YELLOW 0xFFFF00
#define COLOR_GREEN  0x00FF00
#define COLOR_BLUE   0x0000FF
#define COLOR_INDIGO 0x4B0082
#define COLOR_VIOLET 0xEE82EE
#define COLOR_PINK   0xFFCCCC
#define COLOR_WHITE  0xFFFFFF

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN);

StaticJsonBuffer<200> jsonBuffer;

int pos = 0;

void setup() {
  HWSERIAL.begin(38400);
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  pos = 0;
}

void loop() {
  
  int incomingByte;
  String numPixelsStr = "";
  
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();

    // Return health check results in JSON

    numPixelsStr = String(pixels.numPixels());
    JsonObject& root = jsonBuffer.createObject();
    root["name"] = "TeensyPixel1939";
    root["version"] = "0.0.1";
    root["numPixels"] = numPixelsStr;
    root.printTo(HWSERIAL);
    
    pixels.setPixelColor(pos, COLOR_BLACK);
    pos++;
    if (pos == NUMPIXELS) 
      pos = 0;
    pixels.setPixelColor(pos, COLOR_WHITE);
    pixels.show();
  }
}
