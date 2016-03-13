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

/*
 * LED Schedule
 * 
 * 0-29 - starboard side
 *      6-17 shield
 * 30-47 - back
 * 48-77 - port side
 * 54-66 shield
 * 78-103 - front bar
 */

// Include Adafruit's NeoPixel library

#include <Adafruit_NeoPixel.h>

// Debug mode (uncomment next line for debug mode)

//#define DEBUG 1 

// Implementation specific settings

#define SIGNAL0      0  // Pin used for first (least significant) command bit
#define SIGNAL1      1  // Pin used for second command bit
#define SIGNAL2      2  // Pin used for last (most significant) command bit
#define BUILTINLED  13  // Pin assigned to the built-in LED on the Teensy
#define PIN         17  // Pin to use to talk to the NeoPixel Strip
#define BRIGHTNESS  80  // Default brightness level
#define FULLBRIGHT 255  // Full on
#define NUMPIXELS  104  // Number of pixels in strip
#define FLASHDELAY 100  // Time width of flash (on and off time equal)

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

int      command = 0;          // Current command (light program)
int      lastcmd = 0;          // Last command (used to recover from effects that impact all lights
bool     sPin1, sPin2, sPin3;  // Boolean values for pin settings
uint32_t ledValues[NUMPIXELS]; // Local buffer echoing pixel color settings
bool     flash = false;        // Flash state (true = on)
int      pixelstate = 0;       // Indicates which pixel is activiate in rotation. Changes each
                               // round through the event loop.

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
#ifdef DEBUG
  Serial.begin(38400);
  Serial.println("Lighting Program Active");
#endif
  pinMode(BUILTINLED, OUTPUT);      
  pinMode(SIGNAL0,    INPUT);     
  pinMode(SIGNAL1,    INPUT);     
  pinMode(SIGNAL2,    INPUT);     
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  setShield(COLOR_GREEN);
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
  
  sPin1 = digitalRead(SIGNAL0) == HIGH;
  sPin2 = digitalRead(SIGNAL1) == HIGH;
  sPin3 = digitalRead(SIGNAL2) == HIGH;

  // Save last command state and get new one

  lastcmd = command;
  command = getCmd(sPin1, sPin2, sPin3);

  // Print state to console for diagnostics

#ifdef DEBUG
  Serial.print("Pins: ");
  Serial.print(sPin1 ? "1" : "0");
  Serial.print(sPin2 ? "1" : "0");
  Serial.print(sPin3 ? "1" : "0");
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

  // If last command state was a full effect, i.e. used all the LEDs, then
  // recover from that state.  This is another hack and a way to handle the
  // shield without implementing an LED map.
  // TODO: Implement this in a better way

  if ((command != lastcmd) && lastcmd < 3 && command > 2) {
    resetStrip();
  }

  // Display the selected program
  
  switch (command) {
    case 7: // Mode: 111 End Game (Green)
      allOn(COLOR_GREEN);
      break;
    case 6: // Mode 110; Red With Boulder (Red Flashing)
      allOn((flash) ? COLOR_RED : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 5: // Mode 101 Blue With Boulder (Blue Flashing)
      allOn((flash) ? COLOR_BLUE : COLOR_BLACK);
      flash = !flash;
      delay(FLASHDELAY);
      break;
    case 4: // Mode 100 Red No Boulder (Red)
      allOn(COLOR_RED);
      break;
    case 3: // Mode 011 Blue No Boulder (Blue)
      allOn(COLOR_BLUE);
      break;
    case 2: // Mode 010 Auto (Yellow)
      loopAround(COLOR_YELLOW);
      break;
    case 1: // Mode 001 Disabled (Rainbow)
      rainbow2();
      break;
    case 0: // Mode: Disconnected
      allOn(COLOR_ORANGE);
      break;
  }

  // Increment pixel state. Adjust back to zero when appropriate

  pixelstate++;
  if (pixelstate >= NUMPIXELS)
    pixelstate = 0;

}

/*
 * setAPixel() - Set pixel command
 */

void setAPixel(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color, BRIGHTNESS);
}

void setAPixel(uint16_t pixel, uint32_t color, uint8_t level) {
  ledValues[pixel] = color;
  pixels.setBrightness(level);
  pixels.setPixelColor(pixel, color);
}

/*
 * setShield() - set the color of the pixels lighting the shield
 */

void setShield(uint32_t color) {
  
  // Below is a hacky way of handling the shield pixels
  // there is a better way to do this with an array mapping
  // virtual pixels to pixel numbers, but in the heat of
  // competition prep that didn't get done.
  // TODO: Do this in a better way
  
  for (int pixel = 6; pixel < 18; pixel++)
    setAPixel(pixel, color, FULLBRIGHT);
  for (int pixel = 54; pixel < 66; pixel++)
    setAPixel(pixel, color, FULLBRIGHT);
  pixels.show();

}

/*
 * allOn() - Sets all pixels to a specified color
 */

void allOn(uint32_t color) {
  for (int pixel = 0; pixel < NUMPIXELS; pixel++) {

    // And... below is a doubly hacky way of ignoring the shield pixels
    // TODO: Do this in a better way
    
    if (pixel > 5 && pixel < 18)
      continue;
    if (pixel > 53 && pixel < 66)
      continue;
    setAPixel(pixel, color);
  }
  pixels.show();
}

/*
 * resetStrip()
 * 
 * Reset all pixels in the strip to black (i.e. off)
 */

void resetStrip() {
    allOn(COLOR_BLACK);
    setShield(COLOR_GREEN);
}

/*
 * rainbow()
 * 
 * Generate a rainbow chase light effect
 */

void rainbow() {
    const int modeDelay = 25;
    
    setAPixel(pixelstate, colors[0]);
    
    for (int i = 0; i < 5; i++)
      if (pixelstate > i)
        setAPixel(pixelstate - i - 1, colors[i + 1]);

    if (pixelstate > 5)
      setAPixel(pixelstate - 6, COLOR_BLACK);

    if (pixelstate == 4) {
      setAPixel(3, COLOR_BLACK);
      setAPixel(2, COLOR_BLACK);
      setAPixel(1, COLOR_BLACK);
      setAPixel(0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);          
      setAPixel(NUMPIXELS - 2, COLOR_BLACK);          
    }

    if (pixelstate == 3) {
      setAPixel(2, COLOR_BLACK);
      setAPixel(1, COLOR_BLACK);
      setAPixel(0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);
      setAPixel(NUMPIXELS - 2, COLOR_BLACK);     
      setAPixel(NUMPIXELS - 3, COLOR_BLACK);     
    }

    if (pixelstate == 2) {
      setAPixel(1, COLOR_BLACK);
      setAPixel(0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);
      setAPixel(NUMPIXELS - 2, COLOR_BLACK);
      setAPixel(NUMPIXELS - 3, COLOR_BLACK);
      setAPixel(NUMPIXELS - 4, COLOR_BLACK);     
    }
 
    if (pixelstate == 1) {
      setAPixel(0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);
      setAPixel(NUMPIXELS - 2, COLOR_BLACK);
      setAPixel(NUMPIXELS - 3, COLOR_BLACK);
      setAPixel(NUMPIXELS - 4, COLOR_BLACK);
      setAPixel(NUMPIXELS - 5, COLOR_BLACK);
    }

    if (pixelstate == 0) {
      setAPixel(NUMPIXELS - 0, COLOR_BLACK);
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);
      setAPixel(NUMPIXELS - 2, COLOR_BLACK);
      setAPixel(NUMPIXELS - 3, COLOR_BLACK);
      setAPixel(NUMPIXELS - 4, COLOR_BLACK);
      setAPixel(NUMPIXELS - 5, COLOR_BLACK);
      setAPixel(NUMPIXELS - 6, COLOR_BLACK);
    }

    pixels.show();
    delay(modeDelay);
}

/*
 * rainbow()
 * 
 * Generate a rainbow chase light effect
 */

void rainbow2() {
  const int modeDelay = 25;
  for (int pixel = 0; pixel < NUMPIXELS; pixel++) {

    // And... below is a doubly hacky way of ignoring the shield pixels
    // TODO: Do this in a better way
    
    if (pixel > 5 && pixel < 18)
      continue;
    if (pixel > 53 && pixel < 66)
      continue;
    setAPixel(pixel, colors[(int)((float)pixel + pixelstate / 7.25) % 7 ]);
  }
    
  pixels.show();
  delay(modeDelay);
}

/*
 * loopAround()
 * 
 * Loop around the strip once with a specified color
 */

void loopAround(uint32_t color) {
    const int modeDelay = 10;
    if (pixelstate == 0)
      setAPixel(NUMPIXELS - 1, COLOR_BLACK);
    if (pixelstate > 0)
      setAPixel(pixelstate - 1, COLOR_BLACK);
    setAPixel(pixelstate, color);       
    pixels.show();
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


