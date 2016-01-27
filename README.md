# NeoPixel-2016

Repository holding the work the junior developers are undertaking to drive the NeoPixel lights on the 2016 robot.

## The Teensy-LC

To simplify driving [NeoPixels](https://learn.adafruit.com/adafruit-neopixel-uberguide/overview) from the [Raspberry Pi](https://www.raspberrypi.org/), we're experimenting with using a [Teensy-LC](https://www.pjrc.com/teensy/teensyLC.html) (a miniture and enhanced [Arduino](https://www.arduino.cc/) cloan) to drive the NeoPixels based upon commands received from the Raspberry Pi. Code supporting the Teensy-LC is contained in the 'Tinsey-LC Driver' directory.

Why go through all this? Only so that we can teach Java programming skills to junior developers that are transferable to the robot control code which is also in Java.  The Raspberry Pi supports running Java (just like the RoboRIO for the robot).  Having junior developers program the lighting system on the robot in Java is a good way to introduce them to programming.  For the lighting system, we desire to leverage the NeoPixels we already own.  NeoPixels have some quirky characteristics that complicate driving them from a Raspberry Pi.  It can be done, but not so easily right now using Java.  So, the thinking (still to be proven out) is that having the Raspberry Pi command the NeoPixel lighting via the Teensy-LC is the best option.

For now, the only think included is a demonstration of using the Teensy-LC to drive a 12-LED NeoPixel Ring.
