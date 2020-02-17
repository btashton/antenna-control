# Antenna Control Project
It all started here https://twitter.com/btashton/status/1228868489022074880

This is the inital PoC work for a motorized antenna rotator with an IMU based off of a Tasco StarGuide telescope mount I aquired from Goodwill.

## Quality
This is really just thrown together code for now to get something working.  Much of it is random boards and sensor I have glued together with some Arduino code.  Eventually I'll likely move the software to run on Apache NuttX and further extend it from there.

## Getting started
Make sure you have the Teensyduino beta from here https://forum.pjrc.com/threads/59566-Teensyduino-1-51-Beta-1

Then these libraries:
 * https://github.com/Richard-Gemmell/teensy4_i2c
 * https://github.com/adafruit/Adafruit_Sensor
 * https://github.com/btashton/Adafruit_LSM9DS1  (teensy4)
 * https://github.com/btashton/Adafruit_LIS3MDL  (teensy4)
 * https://github.com/btashton/Adafruit_BusIO    (teensy4)

Hardware:
 * Teensy4 board
 * 1.5inch RGB OLED Module (Waveshare)
 * BerryGPS-IMU-3 (v3.3)

TODO: Wiring Guide
