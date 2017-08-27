// Warning, set sinMaxDisabled = 0 for the production firmware
bool sinMaxDisabled = 1; // Set this 0 for testing without transformer

/* 
Note: use the latest Arduino software and install the libraries.

Arduino spot welder controller
http://www.avdweb.nl/arduino/hardware-interfacing/spot-welder-controller.html

Copyright (C) 2012  Albert van Dalen
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .
Version 5.2 1-02-2015
Version 6.0 2-01-2017 added menu with TFT display
Version 6.1 22-05-2017 added set display orientation
Version 6.2 26-8-2017 only supports PCB with LCD now, weldContinuously with sinusMax, displayDot improved

Program with FTDI programmer, Extra > Board > Arduino Uno

              <   20ms   >< >sinusMax_us           
               _____       _____       _____     
zeroCross   __|     |_____|     |_____|     |__                 
                              _____________           
weld       __________________|             |____                                              

T   = 1/(4*50Hz) = 5000us 
T   = 1/(4*60Hz) = 4167us
Average time: sinusMax_us = (5000us + 4167us)/2 = 4583us, is not critical, use for 50 and 60Hz
*/

#include <Arduino.h>
#include <Streaming.h> 
#include <SPI.h>
#include <TFT_22_ILI9225.h> // todo for ILI9341 240 x 320
#include <Albert.h>
#include <Switch.h>  
#include <EEPROMex.h>
#include "Definitions.h"
#include "Classes.h"
#include "Eeprom.h"

String version = "Version 6.2";

Switch weldButton(weldButtonPin);
Switch upButton(upButtonPin);
Switch downButton(downButtonPin);
Switch selectButton(selectButtonPin);
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RSTpin, TFT_RSpin, TFT_CSpin, 0); // use hardware SPI
UpDownValue upDownItemNr = UpDownValue(0, 1, 0, 2); // 3 items 0 1 2 
Menu menu; 
Eeprom eeprom;
// Defines the main menu entry names/items
MenuItem preweldTimeItem = MenuItem("Preweld, ms", UpDownValue(50, 50, 0, 1000)); // value, step, minValue, maxValue
MenuItem pauseTimeItem = MenuItem("Pause, ms", UpDownValue(500, 50, 0, 1000)); 
MenuItem weldTimeItem = MenuItem("Weld time, ms", UpDownValue(250, 50, 0, 1000));
MenuItem menuItems[] = {preweldTimeItem, pauseTimeItem, weldTimeItem};

bool continuously;
unsigned orientation;

void setup() 
{ Serial.begin(9600);
  eeprom.init();
  eeprom.read(); 
  setpinModes();
  setOrientation(); // LCD orientation is random, do after eeprom.init()  
  TFTinit();
  selectContinuously();  
  printValuesToSerial();
  menu.displayAll();
}
  
void loop() 
{ pollAll();
  menu.control();
  weldControl();
}


