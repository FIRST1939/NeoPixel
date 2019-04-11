/*
 * DeepSpace FIRST Robotics Competition (2019)
 * Team 1939 Barstow Kuhnigits Lighting Program
 * Lancelot
 *
 * Hardware configuration leverages an Arduino microcontroller.
 * Commands are sent to the via four digital signal pins from the a National Instruments
 * RoboRIO controller.  This provides sixteen different program settings.
 */

#include <Adafruit_NeoPixel.h>

// Implementation specific settings

#define SIGNAL0        2  // Pin used for first (least significant) command bit
#define SIGNAL1        3  // Pin used for second command bit
#define SIGNAL2        4  // Pin used for third command bit
#define SIGNAL3        5  // Pin used for last (most significant) command bit
#define BUILTINLED    13  // Pin assigned to the built-in LED on the Teensy
#define PIN            6  // Pin to use to talk to the NeoPixel Strip

#define BRIGHTNESS   120  // Default brightness level
#define FULLBRIGHT   255  // Full on

#define NUMPIXELS     69  // Number of pixels in strip
#define FLASHDELAY   200  // Time width of flash (on and off time equal)

// Color definitions RGB strand (as opposed to RGBW strands)
//
// These values are in hex with three two-byte groups, i.e. 0xAABBCC where 'AA', 'BB', and 'CC' are
// each color value ranging from 0x00 to 0xFF.  Note that the "0x" prefix tells the compiler that
// what follows is a number in hexadecimal format.
//
// TODO:  The strand is really GRB, not RGB.  So, these definitions may in fact be incorrect and
//        need to be adjusted.  They may have looked okay at Sedalia because we were only using
//        two and the pin logic is backwards.  It will become apparent as more states are played with

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

// Variables

bool     sPin1;          // Boolean values for pin settings
bool     sPin2;
bool     sPin3;
bool     sPin4;

// Initialize NeoPixel Library

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// setup() - Arduino Setup (this function runs one time when the Arduino is either powered on or reset)

void setup() {

  // Tell the Arduino which pins are outputs and which are inputs
  // (The Neopixel library takes care of the Neopixel pin)

  pinMode(BUILTINLED, OUTPUT);
  pinMode(SIGNAL0,    INPUT);
  pinMode(SIGNAL1,    INPUT);
  pinMode(SIGNAL2,    INPUT);
  pinMode(SIGNAL3,    INPUT);

  // Configure the Neopixel

  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  pixels.show(); // Initializes all pixels to 'off'

  // Turn on the Arduino's lights so we know the Arduino is functional

  digitalWrite(BUILTINLED, HIGH);

}

/*
 * setAPixel() - Set pixel command
 *
 * Note:  This is an overloaded function (look up function overloading).  You can call it with two
 *        parameters (pixel number and color) or you can call it with three parameteres (pixel number,
 *        color, and brightness).  If you call it with only two, the default value in the BRIGHTNESS
 *        constant is used.
 */

void setAPixel(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color, BRIGHTNESS);
}

void setAPixel(uint16_t pixel, uint32_t color, uint8_t level) {
  pixels.setBrightness(BRIGHTNESS);
  pixels.setPixelColor(pixel, color);
}

// setPixelRange() - Set a range of pixels

void setPixelRange(uint32_t color, uint8_t first, uint8_t last) {
  for (int pixel = first; pixel < last; pixel++)
    setAPixel(pixel, color);
}

// loop() - An infinite loop that the Arduino runs over and over again as fast as it can

void loop() {

  // Read signal pins on the digital port
  //
  // NOTE:  Using boolean variables (sPin1 to sPin4) really are not needed in code this simple.
  //        The reason for them is to make the if-elseif-else chain of if statements needed to
  //        support 16 different states.

  sPin1 = digitalRead(SIGNAL0) == HIGH;
  sPin2 = digitalRead(SIGNAL1) == HIGH;
  sPin3 = digitalRead(SIGNAL2) == HIGH;
  sPin4 = digitalRead(SIGNAL3) == HIGH;

  // This is where the seriese of if-elseif-else code goes to 'decode' the different combinations
  // of the four pin states to pick the color programs.
  //
  // NOTE:  For this simple code, we simply run the color program directly.  In a more complex
  //        program that has special effects what we should have are calls to functions where
  //        each function is the desired effect.

  if (sPin1 && sPin2 && sPin3 && sPin4) {
    setPixelRange(COLOR_RED, 0, NUMPIXELS);
  } else {
    setPixelRange(COLOR_GREEN, 0, NUMPIXELS);
  }

  // Display the new pixel settings

  pixels.show();

} // loop()
