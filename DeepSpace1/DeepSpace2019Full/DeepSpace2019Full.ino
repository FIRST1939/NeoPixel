/*
   DeepSpace FIRST Robotics Competition (2019)
   Team 1939 Barstow Kuhnigits Lighting Program
   Lancelot

   Hardware configuration leverages an Arduino microcontroller and a microphone.
   Commands are sent to the via four digital signal pins from the a National Instruments
   RoboRIO controller.  This provides sixteen different program settings.
*/

#include <Adafruit_NeoPixel.h>
// Implementation specific settings

#include <math.h>

// Neopixel Defines

// Microphone Defines

#define MIC_PIN         A0  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot
#define INPUT_FLOOR     34  // Lower range of analogRead input
#define INPUT_CEILING  150  // Max range of analogRead input, the lower the value the more sensitive (1023 = max)




#define SIGNAL0        2  // Pin used for first (least significant) command bit
#define SIGNAL1        3  // Pin used for second command bit
#define SIGNAL2        4  // Pin used for third command bit
#define SIGNAL3        5  // Pin used for last (most significant) command bit
#define BUILTINLED    13  // Pin assigned to the built-in LED on the Teensy
#define PIN            6  // Pin to use to talk to the NeoPixel Strip

#define BRIGHTNESS   120  // Default brightness level
#define FULLBRIGHT   255  // Full on

#define NUMPIXELS_RIGHT 34
#define NUMPIXELS_LEFT 35
#define NUMPIXELS     69// Number of pixels in strip
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

uint32_t colors[] = {
  COLOR_RED,
  COLOR_ORANGE,
  COLOR_YELLOW,
  COLOR_GREEN, 
  COLOR_BLUE, 
  COLOR_INDIGO, 
  COLOR_VIOLET
};

// Variables

bool     sPin1;          // Boolean values for pin settings
bool     sPin2;
bool     sPin3;
bool     sPin4;

// Initialize globals

byte         peak         = 16;  // Peak level of column; used for falling dots
byte         dotCount     =  0;  // Frame counter for peak dot
byte         dotHangCount =  0;  // Frame counter for holding peak dot
unsigned int sample;

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
   setAPixel() - Set pixel command

   Note:  This is an overloaded function (look up function overloading).  You can call it with two
          parameters (pixel number and color) or you can call it with three parameteres (pixel number,
          color, and brightness).  If you call it with only two, the default value in the BRIGHTNESS
          constant is used.
*/
//


void setAPixel(uint16_t pixel, uint32_t color) {

  setAPixel(pixel, color, BRIGHTNESS);

}

int getPixelNum(int pixel) {
  int num = abs(pixel - NUMPIXELS);
  return num;
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

//setPixel() - Sets an individual pixel using RGB

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
#endif
}

// loop() - An infinite loop that the Arduino runs over and over again as fast as it can

void loop() {
  boolean enabledSound = true;

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
    if (enabledSound) {
      int           numPixels = 34;         // Number of pixels on one side
      unsigned long startMillis = millis(); // Start of sample window
      float         peakToPeak = 0;         // peak-to-peak level
      unsigned int  signalMax = 0;
      unsigned int  signalMin = 1023;
      unsigned int  c, y;

      // collect data for length of sample window (in mS)
      while (millis() - startMillis < SAMPLE_WINDOW) {

        sample = analogRead(MIC_PIN);
        if (sample >= 1024) { // toss out spurious readings
          continue;
        }

        if (sample > signalMax) {
          signalMax = sample;  // save just the max levels
        } else if (sample < signalMin) {
          signalMin = sample;  // save just the min levels
        }
      }
      peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

      //Fill the strip with rainbow gradient
      for (int i = 0; i <= numPixels - 1; i++) {
        setBothSides(i, Wheel(map(i, 0, numPixels - 1, 30, 150)));
      }

      //Scale the input logarithmically instead of linearly
      c = fscale(INPUT_FLOOR, INPUT_CEILING, numPixels, 0, peakToPeak, 2);

      // Turn off pixels that are below volume threshold.
      if (c < peak) {
        peak = c;          // Keep dot on top
        dotHangCount = 0;  // make the dot hang before falling
      }

      // Fill partial column with off pixels
      if (c <= numPixels) {
        drawLine(numPixels, numPixels - c, pixels.Color(0, 0, 0));
      }

      // Set the peak dot to match the rainbow gradient
      y = numPixels - peak;
      setBothSides(y - 1, Wheel(map(y, 0, numPixels - 1, 30, 150)));
      pixels.show();

      // Frame based peak dot animation
      if (dotHangCount > PEAK_HANG) { //Peak pause length
        if (++dotCount >= PEAK_FALL) { //Fall rate
          peak++;
          dotCount = 0;
        }
      } else {
        dotHangCount++;
      }
    } else {
       //rainbowCycle(50);
        rainbow();
    }


  } else if (sPin1 && sPin2 && sPin3 && !sPin4) {
    setPixelRange(COLOR_GREEN, 0, NUMPIXELS);
  } else if (sPin1 && sPin2 && !sPin3 && sPin4) {
    setPixelRange(COLOR_RED, 0, NUMPIXELS);
  } else if (sPin1 && !sPin2 && sPin3 && sPin4) {
    Strobe(COLOR_ORANGE, 100, 10, 1000);
  }
  else if (!sPin1 && sPin2 && sPin3 && sPin4) {
    setPixelRange(COLOR_ORANGE, 0, NUMPIXELS);
  }
  else if (!sPin1 && sPin2 && sPin3 && sPin4) {
    setPixelRange(COLOR_ORANGE, 0, NUMPIXELS);
  }
  else if (sPin1 && sPin2 && !sPin3 && !sPin4) {
    Strobe(COLOR_YELLOW, 100, 10, 1000);
  }
  else if (sPin1 && !sPin2 && !sPin3 && sPin4) {
    setPixelRange(COLOR_YELLOW, 0, NUMPIXELS);
  }
  else if (!sPin1 && !sPin2 && sPin3 && sPin4) {
    RunningLights(COLOR_VIOLET, 50);
  }
  else if (!sPin1 && sPin2 && sPin3 && !sPin4) {
    RunningLights(COLOR_YELLOW, 50);
  }
  else if (sPin1 && !sPin2 && sPin3 && !sPin4) {
    RunningLightsDown(COLOR_YELLOW, 50);
  }
  else if (!sPin1 && sPin2 && !sPin3 && sPin4) {
    RunningLights(COLOR_ORANGE, 50);
  }
  else if (sPin1 && !sPin2 && !sPin3 && !sPin4) {
    RunningLightsDown(COLOR_ORANGE, 50);
  }
  else if (!sPin1 && sPin2 && !sPin3 && !sPin4) {
    setPixelRange(COLOR_BLUE, 0, NUMPIXELS);
  }
  else if (!sPin1 && !sPin2 && sPin3 && !sPin4) {
    Strobe(COLOR_BLUE, 100, 10, 1000);
  }
  else if (!sPin1 && !sPin2 && !sPin3 && sPin4) {
    Strobe(COLOR_PINK, 100, 10, 1000);
  }

  // Display the new pixel settings

  pixels.show();

} // loop()

//showStrip() - Displays pixel color changes.
void showStrip() {
  pixels.show();
}


//RunningLihtsMix() - Running lights method that was given in the library uses a method that requires the input of the multple color channels.

void RunningLightsMix(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int j = 0; j < NUMPIXELS_LEFT * 2; j++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUMPIXELS_LEFT; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      // setAPixel(i,COLOR_ORANGE)
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
      setPixel(getPixelNum(i), ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    pixels.show();
    delay(WaveDelay);
  }
}

// RunningLights() - Same as previous method but does not allow multiple color inputs.

void RunningLights(uint32_t color, int WaveDelay) {
  int Position = 0;

  for (int j = 0; j < NUMPIXELS_LEFT * 2; j++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < NUMPIXELS_LEFT; i++) {

      setAPixel(i, color);
      setAPixel(getPixelNum(i), color);

    }

    pixels.show();
    delay(WaveDelay);
  }
}

// RunningLightDowns() - Same as previous method but in opposite direction - does not allow multiple color inputs.

void RunningLightsDown(uint32_t color, int WaveDelay) {
  int Position = 0;

  for (int j = NUMPIXELS_LEFT * 2; j > 0; j--)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = NUMPIXELS_LEFT; i > 0; i--) {

      setAPixel(i, color);
      setAPixel(getPixelNum(i), color);

    }

    pixels.show();
    delay(WaveDelay);
  }
}

//Strobe() - A Strobe method that sets the lights to flash.

void Strobe(uint32_t color, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    setPixelRange(color, 0, NUMPIXELS);
    showStrip();
    delay(FlashDelay);
    setPixelRange(0, 0, NUMPIXELS);
    showStrip();
    delay(FlashDelay);
  }

  delay(EndPause);
}

//setBothSides() - Sets both sides of the strip to the same value.
void setBothSides(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color);
  setAPixel(NUMPIXELS - pixel, color);
}

//drawLine() - Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for (int i = from; i <= to; i++) {
    setBothSides(i, c);
  }
}

// rainbow() - Last years method of creating a rainbow value for the lights -- the 8 was added previously was pixelState
void rainbow() {
  for (int pixel = 0; pixel < NUMPIXELS; pixel++)
    setAPixel(pixel, colors[(int)((float)pixel + 8 / 7.25) % 7 ]);
}


void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUMPIXELS_LEFT; i++) {
      c = WheelRain(((i * 256 / NUMPIXELS_LEFT) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
      setPixel(getPixelNum(i), *c, *(c + 1), *(c + 2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * WheelRain(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

// Scale the microphone signal level
// Includes converting it from exponential levels to linear
// Based on the vumeter code sample for Adafruit Circuitplayground
float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }

  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  } else {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    return  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }

  // invert the ranges
  return newBegin - (pow(normalizedCurVal, curve) * NewRange);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  if (WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  WheelPos -= 170;
  return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}
