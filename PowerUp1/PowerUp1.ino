/*
 * PowerUp FIRST Robotics Competition (2018)
 * Team 1939 Barstow Kuhnigits Lighting Program
 * Sir
 *
 * Hardware configuration leverages a Teensy-LC Arduino clone
 * microcontroller.  Commands are sent to the Teensy-LC via
 * four digital signal pins from the a National Instruments
 * RoboRIO controller.  This provides sixteen different program
 * settings.
 */

/*
 * LED Schedule
 *
 * 000-033 - left side
 * 034-067 - right side
 */

// Include Adafruit's NeoPixel library

#include <Adafruit_NeoPixel.h>

// Debug mode (uncomment next line for debug mode)

// #define DEBUG 1

// Implementation specific settings

#define SIGNAL0      0  // Pin used for first (least significant) command bit
#define SIGNAL1      1  // Pin used for second command bit
#define SIGNAL2      2  // Pin used for third command bit
#define SIGNAL3      4  // Pin used for last (most significant) command bit
#define BUILTINLED  13  // Pin assigned to the built-in LED on the Teensy
#define PIN         17  // Pin to use to talk to the NeoPixel Strip
#define BRIGHTNESS 120  // Default brightness level
#define FULLBRIGHT 255  // Full on
#define NUMPIXELS   70  // Number of pixels in strip
#define FLASHDELAY 200  // Time width of flash (on and off time equal)

// Define standard color patterns (ww gg rr bb)

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

// Constants for fade function

#define FADE_IN  0
#define FADE_OUT 1

// Initialize the NeoPixel library

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Variables

int      command = 0;    // Current command (light program)
int      lastcmd = 0;    // Last command (used to recover from effects that impact all lights
bool     sPin1;          // Boolean values for pin settings
bool     sPin2;
bool     sPin3;
bool     sPin4;
bool     flash     = false; // Flash state (true = on)
bool     chasingUp = false; // Flag indicating chasing direction
int      pixelstate = 0;  // Indicates which pixel is activiate in rotation. Changes each round through the event loop.
int      chasestate = 0;  // Indicates which pixel is active in chase mode

// Rainbow color definitions

uint32_t colors[] = {
  COLOR_RED,
  COLOR_ORANGE,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_BLUE,
  COLOR_INDIGO,
  COLOR_VIOLET
};

/*
 * Arduino set-up function (called once upon power-up and reset)
 */

void setup() {
  pinMode(BUILTINLED, OUTPUT);
  pinMode(SIGNAL0,    INPUT);
  pinMode(SIGNAL1,    INPUT);
  pinMode(SIGNAL2,    INPUT);
  pinMode(SIGNAL3,    INPUT);
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  resetStrip();
  pixels.show(); // Initializes all pixels to 'off'
  flash = false;
  chasingUp = true;
  pixelstate = 0;
  chasestate = 0;
  ledOn();
}

/*
 * getCmd() Helper Function
 *
 * Converts pin settings into commmand
 */

// TODO: Replace getCmd() with a direct read of the digital input ports

int getCmd(bool pin1, bool pin2, bool pin3, bool pin4) {
  if        (!pin1 && !pin2 && !pin3 && !pin4) {
    return 0;
  } else if (!pin1 && !pin2 && !pin3 &&  pin4) {
    return 1;
  } else if (!pin1 && !pin2 &&  pin3 && !pin4) {
    return 2;
  } else if (!pin1 && !pin2 &&  pin3 &&  pin4) {
    return 3;
  } else if (!pin1 &&  pin2 && !pin3 && !pin4) {
    return 4;
  } else if (!pin1 &&  pin2 && !pin3 &&  pin4) {
    return 5;
  } else if (!pin1 &&  pin2 &&  pin3 && !pin4) {
    return 6;
  } else if (!pin1 &&  pin2 &&  pin3 &&  pin4) {
    return 7;
  } else if ( pin1 && !pin2 && !pin3 && !pin4) {
    return 8;
  } else if ( pin1 && !pin2 && !pin3 &&  pin4) {
    return 9;
  } else if ( pin1 && !pin2 &&  pin3 && !pin4) {
    return 10;
  } else if ( pin1 && !pin2 &&  pin3 &&  pin4) {
    return 11;
  } else if ( pin1 &&  pin2 && !pin3 && !pin4) {
    return 12;
  } else if ( pin1 &&  pin2 && !pin3 &&  pin4) {
    return 13;
  } else if ( pin1 &&  pin2 &&  pin3 && !pin4) {
    return 14;
  } else {
    return 15;
  }
  return 1000; // should never hit this
}

/*
 * Arduino event loop
 */

void loop() {

  // Read signal pins on the digital port

  sPin1 = digitalRead(SIGNAL0) == HIGH;
  sPin2 = digitalRead(SIGNAL1) == HIGH;
  sPin3 = digitalRead(SIGNAL2) == HIGH;
  sPin4 = digitalRead(SIGNAL3) == HIGH;

  // Save last command state and get new one

  lastcmd = command;
  command = getCmd(sPin1, sPin2, sPin3, sPin4);

// Print state to console for diagnostics when in debug
#ifdef DEBUG
  Serial.print("Pins: ");
  Serial.print(sPin1 ? "1" : "0");
  Serial.print(sPin2 ? "1" : "0");
  Serial.print(sPin3 ? "1" : "0");
  Serial.print(sPin4 ? "1" : "0");
  Serial.print(", Flash State: ");
  Serial.print((flash) ? "On " : "Off");
  Serial.print(", Pixel State: ");
  Serial.print(pixelstate);
  Serial.print(", Chase State: ");
  Serial.print(chasestate);
  Serial.print(", Command: ");
  Serial.print(command);
  Serial.print(", Last Command: ");
  Serial.print(lastcmd);
  Serial.println();
  delay(10);
#endif

// Program Schedule:
//
//  0 - Driver station not attached (test mode)
//  1 - Robot disabled
//  2 - Autonomous mode (Blue Alliance)
//  3 - Elevator rising (Blue Alliance)
//  4 - Elevator descending (Blue Alliance)
//  5 - Elevator at top (Blue Alliance)
//  6 - Climbing
//  7 - Default (Blue Alliance)
//  8 - Not used
//  9 - Not used
// 10 - Autonomous mode (Red Alliance)
// 11 - Elevator rising (Red Alliance)
// 12 - Elevator descending (Red Alliance)
// 13 - Elevator at top (Red Alliance)
// 14 - Climbing
// 15 - Default (Red Alliance)

// Force test mode when in debug
#ifdef DEBUG
  command = 0;
#endif

  switch (command) {
    case 15: // 15 - Default (Red Alliance)
      allOn(COLOR_RED);
      break;
    case 14: // 14 - Climbing
      allOn((flash) ? COLOR_ORANGE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 13: // 13 - Elevator at top (Red Alliance)
      allOn((flash) ? COLOR_RED : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 12: // 12 - Elevator descending (Red Alliance)
//      chaseDown(COLOR_RED);
      allOn((flash) ? COLOR_RED : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 11: // 11 - Elevator rising (Red Alliance)
      chaseUp(COLOR_RED);
      break;
    case 10: // 10 - Autonomous mode (Red Alliance)
      allOn((flash) ? COLOR_INDIGO : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 9: // 9 - Not used
      allOn(COLOR_WHITE);
      break;
    case 8: // 8 - Not used
      allOn(COLOR_WHITE);
      break;
    case 7: //  7 - Default (Blue Alliance)
      allOn(COLOR_BLUE);
      break;
    case 6: // 6 - Climbing
      allOn((flash) ? COLOR_WHITE  : COLOR_BLACK);
      flash = !flash;
      break;
    case 5: // 5 - Elevator at top (Blue Alliance)
      allOn((flash) ? COLOR_BLUE  : COLOR_BLACK);
      flash = !flash;
      break;
    case 4: // 4 - Elevator descending (Blue Alliance)
//      chaseDown(COLOR_BLUE);
      allOn((flash) ? COLOR_BLUE  : COLOR_BLACK);
      flash = !flash;
      break;
    case 3: // 3 - Elevator rising (Blue Alliance)
      chaseUp(COLOR_BLUE);
      break;
    case 2: //  2 - Autonomous mode (Blue Alliance)
      allOn((flash) ? COLOR_INDIGO : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 1: // 1 - Robot disabled
      allOn(COLOR_ORANGE);
      break;
    case 0: //  0 - Driver station not attached (test mode)
#ifdef DEBUG
      allOn(COLOR_WHITE);
#else
      allOn(COLOR_GREEN);
#endif
      break;
  }
  pixels.show();

  // Increment pixel state. Adjust back to zero when appropriate

  pixelstate++;
  if (pixelstate > NUMPIXELS)
    pixelstate = 0;

  if (chasingUp) {
    chasestate++;
    if (chasestate > 33)
      chasestate = 0;
  } else {
    chasestate--;
    if (chasestate < 0)
      chasestate = 32; // max
  }

}

/*
 * setAPixel() - Set pixel command
 */

void setAPixel(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color, BRIGHTNESS);
}

void setAPixel(uint16_t pixel, uint32_t color, uint8_t level) {
  pixels.setPixelColor(pixel, color);
}

/*
 * allOn() - Sets all pixels to a specified color
 */

void allOn(uint32_t color) {
  setLeft(color);
  setRight(color);
}

/*
 * resetStrip()
 *
 * Reset all pixels in the strip to black (i.e. off)
 */

void resetStrip() {
    allOn(COLOR_BLACK);
}

/*
 * rainbow()
 *
 * Generate a rainbow chase light effect
 */

void rainbow() {
  for (int pixel = 0; pixel < NUMPIXELS; pixel++)
    setAPixel(pixel, colors[(int)((float)pixel + pixelstate / 7.25) % 7 ]);
}

/*
 * chaseUp()
 *
 * A rising chase light on both strips
 */

void chaseUp(uint32_t color) {
  const int BAR_SIZE = 6;

  // Make sure we're going up
  chasingUp = true;

  // Left side

  if (chasestate < BAR_SIZE) {
    setAPixel(33 + 1 + chasestate - BAR_SIZE, COLOR_BLACK);
  } else {
    setAPixel(chasestate - BAR_SIZE, COLOR_BLACK);
  }
  setAPixel(chasestate, color);

  // Right side

  if (chasestate < BAR_SIZE) {
    setAPixel(66 + 1 + chasestate - BAR_SIZE, COLOR_BLACK);
  } else {
    setAPixel(33 + chasestate - BAR_SIZE, COLOR_BLACK);
  }
  setAPixel(33 + chasestate, color);
  delay(50);
}

/*
 * chaseDown()
 *
 * Decending chase light on both strips
 */

void chaseDown(uint32_t color) {
  const int BAR_SIZE = 6;

  // Make sure we're going down
  chasingUp = false;

  // Left side

  if (chasestate < BAR_SIZE) {
    setAPixel(33 + 1 + chasestate - BAR_SIZE, COLOR_BLACK);
  } else {
    setAPixel(chasestate - BAR_SIZE, COLOR_BLACK);
  }
  setAPixel(chasestate, color);

  // Right side

  if (chasestate < BAR_SIZE) {
    setAPixel(66 + 1 + chasestate - BAR_SIZE, COLOR_BLACK);
  } else {
    setAPixel(33 + chasestate - BAR_SIZE, COLOR_BLACK);
  }
  setAPixel(33 + chasestate, color);
  delay(50);
}

/*
 * loopAround()
 *
 * Loop around the strip once with a specified color
 */

void loopAround(uint32_t color) {
    const int modeDelay = 10;
    if (pixelstate == 0)
      setAPixel(NUMPIXELS, COLOR_BLACK);
    if (pixelstate > 0)
      setAPixel(pixelstate - 1, COLOR_BLACK);
    setAPixel(pixelstate, color);
    delay(modeDelay);
}

/*
 * Turn on built-in LED
 */

void ledOn() {
  digitalWrite(BUILTINLED, HIGH);
}

/*
 * Turn off built-in LED
 */

void ledOff() {
  digitalWrite(BUILTINLED, LOW);
}

// * 000-033  - left side

void setLeft(uint32_t color) {
  setPixelRange(color, 0, 35);
}

// * 034-067  - right side

void setRight(uint32_t color) {
  setPixelRange(color, 35, 70);
}


// Set a range of pixels

void setPixelRange(uint32_t color, uint8_t first, uint8_t last) {
  for (int pixel = first; pixel < last; pixel++)
    setAPixel(pixel, color, BRIGHTNESS);
}
