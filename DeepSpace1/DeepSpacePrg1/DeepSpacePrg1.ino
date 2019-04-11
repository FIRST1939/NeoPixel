/*
 * DeepSpace FIRST Robotics Competition (2019)
 * Team 1939 Barstow Kuhnigits Lighting Program
 * Lancelot
 *
 * Hardware configuration leverages an Arduino microcontroller.
 * Commands are sent to the via four digital signal pins from the a National Instruments
 * RoboRIO controller.  This provides sixteen different program settings.
 */

/*
 * LED Schedule
 *
 * 000-034  - left strand
 * 035-068  - right strand
 */

// TODO: Set strand signal pin
// TODO: Double check control pin assignments
// TODO: Do we have RGBW or RGB lights

// Include Adafruit's NeoPixel library

#include <Adafruit_NeoPixel.h>

// Debug mode (uncomment next line for debug mode)

#define DEBUG 1

// Test mode (16 color program with no effects)

#define TEST 1

// Implementation specific settings

#define SIGNAL0      2  // Pin used for first (least significant) command bit
#define SIGNAL1      3  // Pin used for second command bit
#define SIGNAL2      4  // Pin used for third command bit
#define SIGNAL3      5  // Pin used for last (most significant) command bit
#define BUILTINLED  13  // Pin assigned to the built-in LED on the Teensy
#define PIN          7  // Pin to use to talk to the NeoPixel Strip
#define BRIGHTNESS 120  // Default brightness level
#define FULLBRIGHT 255  // Full on
#define NUMPIXELS   69  // Number of pixels in strip
#define FLASHDELAY 200  // Time width of flash (on and off time equal)

// Define standard color patterns (ww gg rr bb)
/*
#define COLOR_BLACK  0x00000000
#define COLOR_RED    0x0000FF00
#define COLOR_ORANGE 0x0046FF00
#define COLOR_YELLOW 0x2394D800
#define COLOR_GREEN  0x00FF0000
#define COLOR_BLUE   0x000000FF
#define COLOR_INDIGO 0x00004141
#define COLOR_VIOLET 0x0000FF64
#define COLOR_PINK   0x0E008C00
#define COLOR_WHITE  0xFF000000
*/

// Define standard color patterns (rr gg bb)

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
bool     flash = false;  // Flash state (true = on)
int      pixelstate = 0; // Indicates which pixel is activiate in rotation. Changes each round through the event loop.

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
  pixels.show(); // Initializes all pixels to 'off'
  flash = false;
  pixelstate = 0;
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
//  command = getCmd(sPin1, sPin2, sPin3, sPin4);

  command = 11;

  // Print state to console for diagnostics

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
  Serial.print(", Command: ");
  Serial.print(command);
  Serial.print(", Last Command: ");
  Serial.print(lastcmd);
  Serial.println();
  delay(500);
#endif

  // Test mode

#ifdef TEST

  switch (command) {
    case 15: // alternate red and black
      allOn((flash) ? COLOR_RED    : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 14:
      allOn((flash) ? COLOR_ORANGE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 13:
      allOn((flash) ? COLOR_YELLOW : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 12:
      allOn((flash) ? COLOR_GREEN  : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 11:
      allOn((flash) ? COLOR_BLUE   : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 10:
      allOn((flash) ? COLOR_INDIGO : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 9:
      allOn((flash) ? COLOR_VIOLET : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 8:
      allOn((flash) ? COLOR_WHITE  : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 7:
      allOn(COLOR_RED);
      break;
    case 6:
      allOn(COLOR_ORANGE);
      break;
    case 5:
      allOn(COLOR_YELLOW);
      break;
    case 4:
      allOn(COLOR_GREEN);
      break;
    case 3:
      allOn(COLOR_BLUE);
      break;
    case 2:
      allOn(COLOR_INDIGO);
      break;
    case 1:
      allOn(COLOR_VIOLET);
      break;
    case 0:
      allOn(COLOR_WHITE);
      break;
  }
  pixels.show();

#else

  // Real mode

  // If last command state was a full effect, i.e. used all the LEDs, then
  // recover from that state.  This is another hack and a way to handle the
  // shield without implementing an LED map.
  // TODO: Implement this in a better way

  resetStrip();

  // Display the selected program

  setGear(COLOR_BLACK);
  switch (command) {
    case 0: // Not enabled
      pixels.setBrightness(BRIGHTNESS);
      allOn(COLOR_VIOLET);
      break;
    case 1: // Not used
      pixels.setBrightness(BRIGHTNESS);
      rainbow();
      break;
    case 2: // Auton (Red Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn((flash) ? COLOR_YELLOW : COLOR_RED);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 3: // Auton (Blue Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn((flash) ? COLOR_YELLOW : COLOR_BLUE);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 4: // Driving (Red Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn(COLOR_RED);
      break;
    case 5: // Driving (Blue Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn(COLOR_BLUE);
      break;
    case 6: // Gear in posession (Red Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn(COLOR_RED);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 7: // Gear in possession (Blue Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn(COLOR_BLUE);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 8: // Gear aligned (limit switch activated) (Red Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn(COLOR_RED);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 9: // Gear aligned (limit switch activated) (Blue Alliance)
      pixels.setBrightness(FULLBRIGHT);
      allOn(COLOR_BLUE);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 10: // Climbing (Red Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn((flash) ? COLOR_WHITE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 11: // Climbing (Blue Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn((flash) ? COLOR_WHITE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 12: // Want fuel (Red Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn((flash) ? COLOR_RED : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 13: // Want fuel (Blue Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn((flash) ? COLOR_BLUE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 14: // Gear and fuel releasing (Blue Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn(COLOR_BLUE);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 15: // Gear and fuel releasing (Red Alliance)
      pixels.setBrightness(BRIGHTNESS);
      allOn(COLOR_RED);
      flash = !flash;
      delay(FLASHDELAY);
      break;
  }

#endif
  pixels.show();

  // Increment pixel state. Adjust back to zero when appropriate

  pixelstate++;
  if (pixelstate >= 135)
    pixelstate = 0;

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
  for (int pixel = 0; pixel < 135; pixel++)
    setAPixel(pixel, colors[(int)((float)pixel + pixelstate / 7.25) % 7 ]);
}

/*
 * loopAround()
 *
 * Loop around the strip once with a specified color
 */

void loopAround(uint32_t color) {
    const int modeDelay = 10;
    if (pixelstate == 0)
      setAPixel(135, COLOR_BLACK);
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

// * Left Side

void setLeft(uint32_t color) {
  setPixelRange(color, 0, 34);
}

// * Right Side

void setRight(uint32_t color) {
  setPixelRange(color, 35, 68);
}

// Set a range of pixels

void setPixelRange(uint32_t color, uint8_t first, uint8_t last) {
  for (int pixel = first; pixel < last; pixel++)
    setAPixel(pixel, color, BRIGHTNESS);
}
