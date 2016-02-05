
/*
 * TeensyPixel1939
 * 
 * Firmware for a Teensy-LC to drive NeoPixels under the direction of
 * a Raspberry Pi communicating via serial using a JSON-formatted 
 * protocol.
 * 
 * Library Documentation:
 *      - https://github.com/adafruit/Adafruit_NeoPixel
 *      - https://www.pjrc.com/teensy/td_uart.html
 */

#include <Adafruit_NeoPixel.h>

#define NEOPIN     17
#define NUMPIXELS  12
#define HWSERIAL   Serial1
#define BUFFERSIZE 255
#define MAXARGS    6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN);

uint8_t ledValues[NUMPIXELS][3];
char    cmdBuffer[(BUFFERSIZE + 1)];
int     args[MAXARGS];
int     numArgs = 0;

void setup() {
  HWSERIAL.begin(38400);
  pixels.setBrightness(255);
  pixels.begin();
  clearPixels();
}

void readSerialString(char* buffer) {
  int numCharsRead = 0;
  char charRead;
  Serial.println("start readSerialString");
  do {
    while (HWSERIAL.available() == 0);
    charRead = HWSERIAL.read();
    buffer[numCharsRead] = charRead;
    numCharsRead++;
    Serial.print(charRead);
  } while (charRead != 13 && numCharsRead < BUFFERSIZE);
  buffer[numCharsRead] = '\0';
  Serial.println("\ndone readSerialString");
}

void parseNums() {
  Serial.println("parseNums()");
  int i = 0;
  int num = 0;
  numArgs = 0;
  if (isdigit(cmdBuffer[i])) {
    do {
      while (isdigit(cmdBuffer[i]) && (cmdBuffer[i] != '\0') && (i < BUFFERSIZE)) {
        num = 10 * num + (cmdBuffer[i++] & 0x0F);
      }
      if (numArgs < MAXARGS) {
        args[numArgs++] = num;
      }
      num = 0;
      while (!isdigit(cmdBuffer[i]) && (cmdBuffer[i] != '\0') && (i < BUFFERSIZE)) {
        i++;
      }
    } while ((cmdBuffer[i] != '\0') && (i < BUFFERSIZE));
  }
}

void healthCheck() {
  HWSERIAL.print("OK: ");
  HWSERIAL.print("TeensyPixel1939");
  HWSERIAL.print(",");
  HWSERIAL.print("0.0.1");
  HWSERIAL.print(",");
  HWSERIAL.print(String(pixels.numPixels()));
  HWSERIAL.println();
}

void setAPixel() {
  if (numArgs != 5) {
    badCommand(2, "Four arguments (pixel, red, green, blue) are required to set an LED color");
  } else if ((uint8_t)args[1] >= NUMPIXELS) {
    badCommand(3, "Specified pixel index exceeds the number of installed pixels");
  } else {
    ledValues[args[1]][0] = (uint8_t)args[2];
    ledValues[args[1]][1] = (uint8_t)args[3];
    ledValues[args[1]][2] = (uint8_t)args[4];        
    pixels.setPixelColor((uint16_t)args[1], ledValues[args[1]][0], ledValues[args[1]][1], ledValues[args[1]][2]);
    pixels.show();
    HWSERIAL.print("OK: Set pixel ");
    HWSERIAL.print((uint16_t)args[1]);
    HWSERIAL.print(" to ");
    HWSERIAL.print(ledValues[args[1]][0]);
    HWSERIAL.print(",");
    HWSERIAL.print(ledValues[args[1]][1]);
    HWSERIAL.print(",");
    HWSERIAL.print(ledValues[args[1]][2]);
    HWSERIAL.println();
  }
}

void clearPixels() {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, COLOR_BLACK);
    ledValues[i][0] = 0;
    ledValues[i][1] = 0;
    ledValues[i][2] = 0;
  }
  pixels.show();
  HWSERIAL.println("Ok: All pixels off");
}

void getAllPixels() {
  for (int i = 0; i < pixels.numPixels(); i++) {
    HWSERIAL.print(i);
    HWSERIAL.print(',');
    HWSERIAL.print(ledValues[i][0]);
    HWSERIAL.print(',');
    HWSERIAL.print(ledValues[i][1]);
    HWSERIAL.print(',');
    HWSERIAL.println(ledValues[i][2]);
  }
  HWSERIAL.println("Ok: Pixel values returned");
}

void shiftUp(bool wrap, int count) {
  uint8_t prior[3];
  uint8_t next[3];
  for (int i = 0; i < count; i++) {
    prior[0] = (wrap) ? ledValues[pixels.numPixels() - 1][0] : 0;
    prior[1] = (wrap) ? ledValues[pixels.numPixels() - 1][1] : 0;
    prior[2] = (wrap) ? ledValues[pixels.numPixels() - 1][2] : 0;  
    for (int i = 0; i < pixels.numPixels(); i++) {
      next[0] = ledValues[i][0];
      next[1] = ledValues[i][1];
      next[2] = ledValues[i][2];
      ledValues[i][0] = prior[0];
      ledValues[i][1] = prior[1];
      ledValues[i][2] = prior[2];
      pixels.setPixelColor(i, prior[0], prior[1], prior[2]);
      prior[0] = next[0];
      prior[1] = next[1];
      prior[2] = next[2];
    }
  }
  pixels.show();
  HWSERIAL.print("Ok: Pixels shifted up ");
  HWSERIAL.print(count);
  HWSERIAL.print(" position");
  HWSERIAL.print((count == 1) ? " " : "s ");
  HWSERIAL.print("with");
  HWSERIAL.print((wrap) ? " " : "out ");
  HWSERIAL.print("wrapping");
  HWSERIAL.println();
}

void shiftDown(bool wrap, int count) {
  uint8_t first[3];
  int lastIndex;
  for (int i = 0; i < count; i++) {
    first[0] = ledValues[0][0];
    first[1] = ledValues[0][1];
    first[2] = ledValues[0][2];
    for (int i = 0; i < (pixels.numPixels() - 1); i++) {
      ledValues[i][0] = ledValues[i+1][0];
      ledValues[i][1] = ledValues[i+1][1];
      ledValues[i][2] = ledValues[i+1][2];
      pixels.setPixelColor(i, ledValues[i][0], ledValues[i][1], ledValues[i][2]);
    }
    lastIndex = NUMPIXELS - 1;
    ledValues[lastIndex][0] = (wrap) ? first[0] : 0;
    ledValues[lastIndex][1] = (wrap) ? first[1] : 0;
    ledValues[lastIndex][2] = (wrap) ? first[2] : 0;
    pixels.setPixelColor(lastIndex, ledValues[lastIndex][0], ledValues[lastIndex][1], ledValues[lastIndex][2]);
  }
  pixels.show();
  HWSERIAL.print("Ok: Pixels shifted down ");
  HWSERIAL.print(count);
  HWSERIAL.print(" position");
  HWSERIAL.print((count == 1) ? " " : "s ");
  HWSERIAL.print("with");
  HWSERIAL.print((wrap) ? " " : "out ");
  HWSERIAL.print("wrapping");
  HWSERIAL.println();
}

void refresh() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, ledValues[i][0], ledValues[i][1], ledValues[i][2]);
  }
  pixels.show();
}

void setBrightnessLevel(int value) {
  pixels.setBrightness((uint8_t)value);
  refresh();
  HWSERIAL.print("Ok: Brightness level set to ");
  HWSERIAL.print(value);
  HWSERIAL.println();
}

void getBrightnessLevel() {
  HWSERIAL.print(pixels.getBrightness());
  HWSERIAL.println();
  HWSERIAL.println("Ok: Brightness level returned");
}

void setRange() {
  if (numArgs != 4 && numArgs != 6) {
    badCommand(4, "Either five arguments (first, last, red, green, blue) are required to set a range of LED color or three (red, green, blue) to set them all");
  } else if (numArgs == 6 && (uint16_t)args[1] < 0) {
    badCommand(5, "First pixel cannot be a negative number");  
  } else if (numArgs == 6 && (uint16_t)args[2] >= NUMPIXELS) {
    badCommand(6, "Last pixel index exceeds the number of installed pixels where '0' is the first pixel");
  } else if (numArgs == 6 && args[1] > args[2]) {
    badCommand(7, "Last pixel index value must be greater than the first pixel index value");
  } else {
    if (numArgs == 4) {
      args[5] = args[3];
      args[4] = args[2];
      args[3] = args[1];
      args[2] = NUMPIXELS;
      args[1] = 0;
    }
    for (uint16_t i = args[1]; i <= args[2]; i++) {
      Serial.print("index ");
      Serial.println(i);
      ledValues[i][0] = (uint8_t)args[3];
      ledValues[i][1] = (uint8_t)args[4];
      ledValues[i][2] = (uint8_t)args[5];
      pixels.setPixelColor(i, ledValues[i][0], ledValues[i][1], ledValues[i][2]);
    }
    pixels.show();
  }
}

void badCommand(int errNum, const char* errMsg) {
  Serial.println("badCommand()");
  HWSERIAL.print("Error (");
  HWSERIAL.print(errNum);
  HWSERIAL.print("): ");
  HWSERIAL.print(errMsg);
  HWSERIAL.println();
  HWSERIAL.print("Received : ");
  HWSERIAL.print(cmdBuffer);
  HWSERIAL.println();
}

void loop() {
  
  if (HWSERIAL.available() > 0) {
    readSerialString(cmdBuffer);
    parseNums();
    if (numArgs == 0) {
      badCommand(0, "No command received");
    } else if (args[0] == 0) {
      healthCheck();
    } else if (args[0] == 1) {
      setAPixel();
    } else if (args[0] == 2) {
      getAllPixels();
    } else if (args[0] == 3) {
      clearPixels();
    } else if (args[0] == 4 && numArgs == 1) {
      shiftUp(false, 1);
    } else if (args[0] == 4 && numArgs == 2) {
      shiftUp(args[1] > 0, 1);
    } else if (args[0] == 4 && numArgs == 3) {
      shiftUp(args[1] > 0, args[2]);
    } else if (args[0] == 5 && numArgs == 1) {
      shiftDown(false, 1);
    } else if (args[0] == 5 && numArgs == 2) {
      shiftDown(args[1] > 0, 1);
    } else if (args[0] == 5 && numArgs == 3) {
      shiftDown(args[1] > 0, args[2]);
    } else if (args[0] == 6 && numArgs == 1) {
      setBrightnessLevel(255);
    } else if (args[0] == 6 && numArgs == 2) {
      setBrightnessLevel(args[1]);
    } else if (args[0] == 7) {
      getBrightnessLevel();
    } else if (args[0] == 8) {
      setRange();
    } else {
      badCommand(1, "Unknown command");
    }
  }
}
