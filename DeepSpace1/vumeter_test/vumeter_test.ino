/*
 * Neopixel VU Meter
 */

#include <Adafruit_NeoPixel.h>
#include <math.h>

// Neopixel Defines

#define NEO_PIN      2  // Pin to use to talk to the NeoPixel Strip
#define BRIGHTNESS  40  // Default brightness level
#define NUMPIXELS   60  // Number of pixels in strip

// Microphone Defines

#define MIC_PIN         A0  // Microphone is attached to this analog pin (A4 for circuit playground)
#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot
#define INPUT_FLOOR     34  // Lower range of analogRead input
#define INPUT_CEILING  150  // Max range of analogRead input, the lower the value the more sensitive (1023 = max)

// Initialize globals

byte         peak         = 16;  // Peak level of column; used for falling dots
byte         dotCount     =  0;  // Frame counter for peak dot
byte         dotHangCount =  0;  // Frame counter for holding peak dot
unsigned int sample;

// Initialize the NeoPixel library

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Set a single pixel at the default brightness
void setAPixel(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color, BRIGHTNESS);
}

// Set a single pixel at a specified brightness level
void setAPixel(uint16_t pixel, uint32_t color, uint8_t level) {
  pixels.setBrightness(level);
  pixels.setPixelColor(pixel, color);
}

// Make the two sides of the strip work as one
void setBothSides(uint16_t pixel, uint32_t color) {
  setAPixel(pixel, color);
  setAPixel(NUMPIXELS - pixel, color);
}

// Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    setBothSides(i, c);
  }
}


// Scale the microphone signal level
// Includes converting it from exponential levels to linear
// Based on the vumeter code sample for Adafruit Circuitplayground
float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve){

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

  if (invFlag == 0){
    return  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }
  
  // invert the ranges
  return newBegin - (pow(normalizedCurVal, curve) * NewRange);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  
  WheelPos -= 170;
  return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}

// Arduino set-up

void setup() {
  pixels.begin();
}

// Arduino event looup

void loop() {

  int           numPixels = 30;         // Number of pixels on one side
  unsigned long startMillis= millis();  // Start of sample window
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
  for (int i=0;i<=numPixels-1;i++){
    setBothSides(i,Wheel(map(i,0,numPixels-1,30,150)));
  }

  //Scale the input logarithmically instead of linearly
  c = fscale(INPUT_FLOOR, INPUT_CEILING, numPixels, 0, peakToPeak, 2);

  // Turn off pixels that are below volume threshold.
  if(c < peak) {
    peak = c;          // Keep dot on top
    dotHangCount = 0;  // make the dot hang before falling
  }
  
  // Fill partial column with off pixels
  if (c <= numPixels) { 
    drawLine(numPixels, numPixels-c, pixels.Color(0, 0, 0));
  }

  // Set the peak dot to match the rainbow gradient
  y = numPixels - peak;
  setBothSides(y-1,Wheel(map(y,0,numPixels-1,30,150)));
  pixels.show();

  // Frame based peak dot animation
  if(dotHangCount > PEAK_HANG) { //Peak pause length
    if(++dotCount >= PEAK_FALL) { //Fall rate
      peak++;
      dotCount = 0;
    }
  } else {
    dotHangCount++;
  }
}



