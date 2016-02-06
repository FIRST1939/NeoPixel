
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

/* TeensyPixel1939 accepts simple numeric command strings from the HWSERIAL serial port of a Teensy-LC
 * then processes them to manipulate a set of NeoPixels (up to NUMPIXELS) connected to the Teensy-LC's
 * port NEOPIN.
 *
 * The commands consist of a command followed by an optional number of parameters. Successful command
 * execution results in an acknowledgement string writen back out to HWSERIAL serial port.  Error messages
 * are provided for invalid on incorrect commands.
 *
 * The following commands are currently supported:
 *
 * 0 - Health Check
 *
 *      Returns the firmware name ("TeensyPixel1939"), version number, and number of pixels.  It is
 *      intended to be used by the host to determine if the correct firmware version is installed as
 *      expected and to return the number of pixels supported.
 *
 *      Example :  0<return>
 *
 *      Response:  Ok: TeensyPixel1939,0.1.0,12
 *
 * 1 - Set Pixel
 *
 *      Sets the color value of a specified pixel. Requires four parameters consisting of the number
 *      of the pixel to set (<pixel#>) along with the red (<red>), green (<green>), and blue(<blue>)
 *      color values.  Pixel counting starts at the number zero, so '1' is the second pixel in the
 *      strand.  Color values are a number ranging from '0' to '255'.
 *
 *      Example :  1,2,0,255,128
 *
 *      Response:  Ok: Set pixel 2 to 0,255,128
 *
 * 2 - Get Pixel Values
 *
 *      Returns a list of the color values of each of the pixels with one line sent back to the host
 *      for each pixel.  Each line consists of the number of the pixel followed by the red, green, and
 *      blue color values.
 *
 *      Example :  2
 *
 *      Response:  0,0,0,0
 *                 1,0,0,0
 *                 2,0,255,128
 *                 3,0,0,0
 *                    ...
 *                 12,0,0,0
 *                 Ok: Pixel values returned
 *
 *  3 - Clear Pixels
 *
 *      Resets all of the pixels back to black (off)
 *
 *      Example :  3
 *
 *      Response:  Ok: All pixels off
 *
 *  4 - Shift Up
 *
 *      Shifts all pixels upwards (lower numbered pixels to higher) a specified number of times
 *      either with, or without, wrapping.  The argument order is the command, followed by the
 *      wrapping value, followed by the count.  Wrapping value and count are optional arguments.
 *      If a wrapping value is not provided, then no wrapping occurs.  If a count value is not
 *      provided, then only one shift occurs.  A wrapping value of '0' indicates no wrapping.
 *      Any other value causes wrapping to occur.  When wrapping is enabled, the highest pixel
 *      value is shifted into the lowest value causing the pixels to "wrap" around.
 *
 *      Example : 4
 *
 *      Response: Ok: Pixels shifted up 1 position without wrapping
 *
 *      Example : 4,0
 *
 *      Response: Ok: Pixels shifted up 1 position without wrapping
 *
 *      Example : 4,0,10
 *
 *      Response: Ok: Pixels shifted up 10 positions without wrapping
 *
 *      Example : 4,2,5
 *
 *      Response: Ok: Pixels shifted up 5 positions with wrapping
 *
 *      Example : 4,1
 *
 *      Response: Ok: Pixels shifted up 1 position with wrapping
 *
 *  5 - Shift Down
 *
 *      Functions exactly like "Shift Up" but in the opposite direction (higher numbered pixels
 *      are shifted to lower positions)
 *
 *      Example : 5,0,2
 *
 *      Response: Ok: Pixels shifted down 2 positions without wrapping
 *
 *  6 - Set Brightness
 *
 *      Sets the brightness level of the pixels.  Accepts an optional argument that specifies the
 *      brightness level with '0' being off and '255' being full brightness.  If the argument is
 *      omitted, the lefel is set to full brightness.
 *
 *      Example : 6
 *
 *      Response: Ok: Brightness level set to 255
 *
 *      Example : 6,100
 *
 *      Response: Ok: Brightness level set to 100
 *
 *  7 - Get Brightness
 *
 *      Returns the current brighness level.
 *
 *      Example : 7
 *
 *      Response: 255
 *                Ok: Brightness level returned
 *
 *  8 - Set a Range of Pixels
 *
 *      Sets a range of pixels to a specified color value.  Accepts either three or five arguments.
 *      When three arguments are passed, all of NUMPIXELS are set to the same R, G, B color value according
 *      the the three values passed.  When five arguments are passed, the first two determine the range
 *      of the pixels whose values are then set to the R, G, B color values passed in the next three
 *      arguments.
 *
 *      Example :  8,255,255,255
 *
 *      Response:  Ok: Pixels 0 through 12 set to 255,255,255
 *
 *      Example :  8,2,5,123,12,34
 *
 *      Response:  Ok: Pixels 2 through 5 set to 123,12,34
 */

 #include <Adafruit_NeoPixel.h>

// Define constants

#define NEOPIN     17       // Pin driving NeoPixels
#define NUMPIXELS  12       // Number of driven NeoPixels attached to pin
#define HWSERIAL   Serial1  // Serial interface ('Serial1' for Teensy-LC)
#define BUFFERSIZE 255      // Maximum size of HWSERIAL receive buffer
#define MAXARGS    6        // Maximum number of passed arguments supported

// Instanciate NeoPixels object connected to NEOPIN containing NUMPIXELS pixels

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN);

// Global variables
//
// NOTE: TeensyPixel1939 maintains a local copy of what the RGB color settings are
//       for each of NUMPIXELS pixels. This is done to retain the original settings
//       unadjusted for brightness levels.  The settings returned by 'pixels.getPixelColor()'
//       are already adjusted per the brightness level.  This local copy also helps
//       with pixel manipulations

uint8_t ledValues[NUMPIXELS][3];      // Local buffer echoing pixel color settings
char    cmdBuffer[(BUFFERSIZE + 1)];  // Buffer for characters received from HWSERIAL
int     args[MAXARGS];                // Parsed argument values
int     numArgs = 0;                  // Number of arguments received


/*
 * setup() - Arduino set-up method
 *
 * Used to configure serial port speed, start the serial port UART, start the NeoPixels
 * routines, set things up to full brightness level, and clear all the pixels.
 */

void setup() {
    HWSERIAL.begin(38400);
    pixels.setBrightness(255);
    pixels.begin();
    noComm();
}

/*
 * readSerialString() - Read a char string from HWSERIAL into command buffer
 *
 * Reads a character string from HWSERIAL until a carriage return ('\n') is found,
 * saves string into the passed buffer, and insures it is terminated with '\0'.
 */


void readSerialString(char* buffer) {
    int numCharsRead = 0;
    char charRead;
    do {
        while (HWSERIAL.available() == 0);
        charRead = HWSERIAL.read();
        buffer[numCharsRead] = charRead;
        numCharsRead++;
    } while (charRead != 13 && numCharsRead < BUFFERSIZE);
    buffer[numCharsRead] = '\0';
}

/*
 * parseNums() - Parse command buffer into number values
 *
 * Parses through the command buffer expecting it to be in the form of
 * "<num>,<num>,<num>,etc.".  Up to MAXARGS arguments are supported.
 * Whitespace and delimiters between numbers can be anything.  parseNums()
 * simply pulls out the numbers with anything as a delimiter.
 */

void parseNums() {
    int i = 0;
    int num = 0;
    numArgs = 0;
    if (isdigit(cmdBuffer[i])) do {
        while (isdigit(cmdBuffer[i]) && (cmdBuffer[i] != '\0') && (i < BUFFERSIZE))
            num = 10 * num + (cmdBuffer[i++] & 0x0F);
        if (numArgs < MAXARGS) args[numArgs++] = num;
        num = 0;
        while (!isdigit(cmdBuffer[i]) && (cmdBuffer[i] != '\0') && (i < BUFFERSIZE)) i++;
    } while ((cmdBuffer[i] != '\0') && (i < BUFFERSIZE));
}

/*
 * healthCheck() - Health check command
 */

void healthCheck() {
    clearPixels();
    HWSERIAL.print("OK: ");
    HWSERIAL.print("TeensyPixel1939");
    HWSERIAL.print(",");
    HWSERIAL.print("0.1.0");
    HWSERIAL.print(",");
    HWSERIAL.print(String(pixels.numPixels()));
    HWSERIAL.println();
}

/*
 * setAPixel() - Set pixel command
 */

void setAPixel() {
    if (numArgs != 5)
        badCommand(2, "Four arguments (pixel, red, green, blue) are required to set an LED color");
    else if ((uint8_t)args[1] >= NUMPIXELS)
        badCommand(3, "Specified pixel index exceeds the number of installed pixels");
    else {
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

/*
 * clearPixels() - Clear all pixels command
 */

void clearPixels() {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, 0, 0, 0);
        ledValues[i][0] = 0;
        ledValues[i][1] = 0;
        ledValues[i][2] = 0;
    }
    pixels.show();
    HWSERIAL.println("Ok: All pixels off");
}

/*
 * noComm() - Communications not yet initiated
 */

void noComm() {
    for (int i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, 255, 0, 0);
        ledValues[i][0] = 255;
        ledValues[i][1] = 255;
        ledValues[i][2] = 0;
    }
    setBrightnessLevel(20);
    pixels.show();
}

/*
 * getAllPixels() - Get and return the value of all pixels command
 */

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

/*
 * shiftUp() - Shift pixels up command
 */

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

/*
 * shiftDown() - Shift pixels down command
 */

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

/*
 * refresh() - Refresh all pixels
 *
 * Helper function to update all pixels with the value retained locally.  Used to properly implement
 * setBrightnessLevel().
 */

void refresh() {
    for (int i = 0; i < NUMPIXELS; i++)
        pixels.setPixelColor(i, ledValues[i][0], ledValues[i][1], ledValues[i][2]);
    pixels.show();
}

/*
 * setBrightnessLevel() - Set brightness level command
 *
 * Accurately sets the brighness level of all pixels. The setBrightness() funcdtion of the NeoPixel
 * library cannot be used in a non-desructive way.  This version works properly.
 */

void setBrightnessLevel(int value) {
    pixels.setBrightness((uint8_t)value);
    refresh();
    HWSERIAL.print("Ok: Brightness level set to ");
    HWSERIAL.print(value);
    HWSERIAL.println();
}

/*
 * getBrightnessLevel() - Get the current brightness level command
 */

void getBrightnessLevel() {
    HWSERIAL.print(pixels.getBrightness());
    HWSERIAL.println();
    HWSERIAL.println("Ok: Brightness level returned");
}

/*
 * setRange() - Set a range of pixels command
 */

void setRange() {
    if (numArgs != 4 && numArgs != 6)
        badCommand(4, "Either five arguments (first, last, red, green, blue) are required to set a range of LED color or three (red, green, blue) to set them all");
    else if (numArgs == 6 && (uint16_t)args[1] < 0)
        badCommand(5, "First pixel cannot be a negative number");
    else if (numArgs == 6 && (uint16_t)args[2] >= NUMPIXELS)
        badCommand(6, "Last pixel index exceeds the number of installed pixels where '0' is the first pixel");
    else if (numArgs == 6 && args[1] > args[2])
        badCommand(7, "Last pixel index value must be greater than the first pixel index value");
    else {
        if (numArgs == 4) {
            args[5] = args[3];
            args[4] = args[2];
            args[3] = args[1];
            args[2] = NUMPIXELS;
            args[1] = 0;
        }
        for (uint16_t i = args[1]; i <= args[2]; i++) {
            ledValues[i][0] = (uint8_t)args[3];
            ledValues[i][1] = (uint8_t)args[4];
            ledValues[i][2] = (uint8_t)args[5];
            pixels.setPixelColor(i, ledValues[i][0], ledValues[i][1], ledValues[i][2]);
        }
        pixels.show();
        HWSERIAL.print("Ok: ");
        HWSERIAL.print("Pixels ");
        HWSERIAL.print((uint16_t)args[1]);
        HWSERIAL.print(" through ");
        HWSERIAL.print((uint16_t)args[2]);
        HWSERIAL.print(" set to ");
        HWSERIAL.print((uint8_t)args[3]);
        HWSERIAL.print(",");
        HWSERIAL.print((uint8_t)args[4]);
        HWSERIAL.print(",");
        HWSERIAL.print((uint8_t)args[5]);
        HWSERIAL.println();
    }
}

/*
 * badCommand() - Helper function to handle returning error messages
 */

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

/*
 * loop() - Arduino main event loop
 */

void loop() {
    if (HWSERIAL.available() > 0) {
        readSerialString(cmdBuffer);
        parseNums();
        if (numArgs == 0)
            badCommand(0, "No command received");
        else if (args[0] == 0)
            healthCheck();
        else if (args[0] == 1)
            setAPixel();
        else if (args[0] == 2)
            getAllPixels();
        else if (args[0] == 3)
            clearPixels();
        else if (args[0] == 4 && numArgs == 1)
            shiftUp(false, 1);
        else if (args[0] == 4 && numArgs == 2)
            shiftUp(args[1] > 0, 1);
        else if (args[0] == 4 && numArgs == 3)
            shiftUp(args[1] > 0, args[2]);
        else if (args[0] == 5 && numArgs == 1)
            shiftDown(false, 1);
        else if (args[0] == 5 && numArgs == 2)
            shiftDown(args[1] > 0, 1);
        else if (args[0] == 5 && numArgs == 3)
            shiftDown(args[1] > 0, args[2]);
        else if (args[0] == 6 && numArgs == 1)
            setBrightnessLevel(255);
        else if (args[0] == 6 && numArgs == 2)
            setBrightnessLevel(args[1]);
        else if (args[0] == 7)
            getBrightnessLevel();
        else if (args[0] == 8)
            setRange();
        else
            badCommand(1, "Unknown command");
    }
}
