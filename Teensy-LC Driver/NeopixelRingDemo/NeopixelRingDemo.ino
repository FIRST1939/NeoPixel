
/*
 * Demonstrate the NeoPixel Ring in action 
 * 
 * NOTE: The inspiration and original source for this demo came from 'energeticspark'. The original work can be found
 *       here: https://github.com/energeticspark/Neopixel-Ring.  This version is simplified and streamlined a bit.
 */

#include <Adafruit_NeoPixel.h>

// Implementation specific settings

#define PIN        17  // Pin to use to talk to the NeoPixel Ring
#define BRIGHTNESS 20  // Brightness level
#define NUMPIXELS  12  // Number of pixels in ring

// Define our color patterns

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

// Initialize NeoPixel library

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN);

// Set things up

void setup() {
    pixels.setBrightness(BRIGHTNESS);
    pixels.begin();
}

// Sketch event loop

void loop() {
    rainbow();
    rainbow();
    loopAround(COLOR_RED);
    loopAround(COLOR_GREEN);
    loopAround(COLOR_BLUE);
    loopAround(COLOR_WHITE);
    pulse(COLOR_RED);
    pulse(COLOR_GREEN);
    pulse(COLOR_BLUE);
    pulse(COLOR_WHITE);
}

// Reset all pixels in the ring to black (i.e. off)

void resetRing() {
    for(int pixel = 0; pixel < NUMPIXELS; pixel++) {
        pixels.setPixelColor(pixel, COLOR_BLACK);
        pixels.setBrightness(BRIGHTNESS);
    }
    pixels.show();
}

// Generate a rainbow chase light effect

void rainbow() {

    static int modeDelay = 100;
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

// Helper function for 'pulse' that either fades in or out all the pixels

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

// Pulse the ring with a specified color

void pulse(uint32_t color) {
    static int modeDelay = 50;
    fade(color, FADE_IN, modeDelay);
    fade(color, FADE_OUT, modeDelay);
    resetRing();       
}

// Loop around the ring once with a specified color

void loopAround(uint32_t color) {
    static int modeDelay = 100;
    for(uint32_t pixel = 0; pixel < NUMPIXELS; pixel++) {
        if (pixel > 0) pixels.setPixelColor(pixel - 1, COLOR_BLACK);
        pixels.setPixelColor(pixel, color);       
        pixels.show();
        delay(modeDelay); 
    }
    resetRing();
}
