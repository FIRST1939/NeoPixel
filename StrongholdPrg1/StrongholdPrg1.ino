/*
 * Stronghold FIRST Robotics Competition (2016)
 * Team 1939 Barstow Kuhnigits Lighting Program
 * Trojan Rabot
 * 
 * Hardware configuration leverages a Teensy-LC Arduino clone
 * microcontroller.  Commands are sent to the Teensy-LC via
 * three digital signal pins from the a National Instruments
 * RoboRIO controller.  This provides eight different program
 * settings.
 */

// Include Adafruit's NeoPixel library

#include <Adafruit_NeoPixel.h>

// Implementation specific settings

#define SIGNAL0      0  // Pin used for first (least significant) command bit
#define SIGNAL1      1  // Pin used for second command bit
#define SIGNAL2      2  // Pin used for last (most significant) command bit
#define BUILTINLED  13  // Pin assigned to the built-in LED on the Teensy
#define PIN         17  // Pin to use to talk to the NeoPixel Ring
#define BRIGHTNESS 255  // Brightness level
#define NUMPIXELS  104  // Number of pixels in ring

// Define standard color patterns

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

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

// Variables

int command = 0;           // Current command (light program)
bool spin1, spin2, spin3;  // Boolean values for pin settings

/*
 * Arduino set-up function (called once upon power-up and reset)
 */

void setup() {
  Serial.begin(9600);
  pinMode(BUILTINLED, OUTPUT);      
  pinMode(SIGNAL0,    INPUT);     
  pinMode(SIGNAL1,    INPUT);     
  pinMode(SIGNAL2,    INPUT);     
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
}

/*
 * getCmd() Helper Function
 * 
 * Converts pin settings into commmand
 */

// TODO: Replace getCmd() with a direct read of the digital input ports

int getCmd(bool pin1, bool pin2, bool pin3) {
  if        (!pin1 && !pin2 && !pin3) {
    return 0;
  } else if (!pin1 && !pin2 &&  pin3) {
    return 1;
  } else if (!pin1 &&  pin2 && !pin3) {
    return 2;
  } else if (!pin1 &&  pin2 &&  pin3) {
    return 3;
  } else if ( pin1 && !pin2 && !pin3) {
    return 4;
  } else if ( pin1 && !pin2 &&  pin3) {
    return 5;
  } else if ( pin1 &&  pin2 && !pin3) {
    return 6;
  } else {
    return 7;    
  }
  return 1000;
}

/*
 * Arduino event loop
 */

void loop() {

  // Read signal pins on the digital port
  
  spin1 = digitalRead(SIGNAL0) == HIGH;
  spin2 = digitalRead(SIGNAL1) == HIGH;
  spin3 = digitalRead(SIGNAL2) == HIGH;

  // Print state to console for diagnostics

  Serial.print(spin1 ? "1" : "0");
  Serial.print(spin2 ? "1" : "0");
  Serial.print(spin3 ? "1" : "0");
  Serial.println();

  // Display the selected program
  
  switch (getCmd(spin1, spin2, spin3)) {
    case 0:
      allOn(COLOR_PINK);
      break;
    case 1:
      allOn(COLOR_RED);
      break;
    case 2:
      allOn(COLOR_BLUE);
      break;
    case 3:
      allOn(COLOR_GREEN);
      break;
    case 4:
      allOn(COLOR_ORANGE);
      break;
    case 5:
      allOn(COLOR_YELLOW);
      break;
    case 6:
      allOn(COLOR_VIOLET);
      break;
    case 7:
      pulse(COLOR_WHITE);
      break;
  }
}

/*
 * allOn() - Sets all pixels to a specified color
 */

void allOn(uint32_t color) {
  for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
    pixels.setBrightness(BRIGHTNESS);
    pixels.setPixelColor(pixel, color);
  }
  pixels.show();
}

/*
 * resetRing()
 * 
 * Reset all pixels in the ring to black (i.e. off)
 */

void resetRing() {
    for(int pixel = 0; pixel < NUMPIXELS; pixel++) {
        pixels.setPixelColor(pixel, COLOR_BLACK);
        pixels.setBrightness(BRIGHTNESS);
    }
    pixels.show();
}

/*
 * rainbow()
 * 
 * Generate a rainbow chase light effect
 */

void rainbow() {

    static int modeDelay = 25;
    
    static uint32_t color[] = {
        COLOR_RED,
        COLOR_ORANGE,
        COLOR_YELLOW,
        COLOR_GREEN, 
        COLOR_BLUE, 
        COLOR_INDIGO, 
        COLOR_VIOLET
    };

    for(int pixel = 0; pixel < NUMPIXELS; pixel++) {
        pixels.setPixelColor(pixel, color[0]);
        for (int i = 0; i < 5; i++)
            if (pixel > i) pixels.setPixelColor(pixel - i - 1, color[i + 1]);
        pixels.setPixelColor(pixel - 6, COLOR_BLACK);
        pixels.show(); 
        delay(modeDelay); 
    }

    for(int pixel = 6; pixel > 0; pixel--) {
        pixels.setPixelColor(NUMPIXELS - pixel, COLOR_BLACK);    
        pixels.show();
        delay(modeDelay);        
    }

    resetRing();
}

/*
 * fade()
 * 
 * Helper function for 'pulse' that either fades in or out all the pixels
 */

void fade(uint32_t color, int fade, int delayBy) {
    int level;
    for (level = (fade ? BRIGHTNESS : 0); (fade ? (level > 0) : (level < BRIGHTNESS)); level = level + (fade ? -1 : 1)) {
        for(int pixel = 0; pixel < NUMPIXELS; pixel++) {
            pixels.setPixelColor(pixel, color);
            pixels.setBrightness(level);
        }
        pixels.show();
        delay(delayBy);
    }
}

/*
 * pulse()
 * 
 * Pulse the ring with a specified color
 */

void pulse(uint32_t color) {
    static int modeDelay = 10;
    fade(color, FADE_IN, modeDelay);
    fade(color, FADE_OUT, modeDelay);
    resetRing();       
}

/*
 * loopAround()
 * 
 * Loop around the ring once with a specified color
 */

void loopAround(uint32_t color) {
    static int modeDelay = 10;
    for(uint32_t pixel = 0; pixel < NUMPIXELS; pixel++) {
        if (pixel > 0) pixels.setPixelColor(pixel - 1, COLOR_BLACK);
        pixels.setPixelColor(pixel, color);       
        pixels.show();
        delay(modeDelay); 
    }
    resetRing();
}
