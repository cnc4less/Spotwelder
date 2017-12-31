bool sinMaxDisabled = 0; // 0 for production firmware, 1 for test without transformer
/* 
Attention: verify code after upload: ON / OFF
 
Todo 
Change maximum values, just as selectTFTorientation()

Note: use the latest Arduino software and install the libraries.

Arduino spot welder controller
http://www.avdweb.nl/arduino/hardware-interfacing/spot-welder-controller.html

Copyright (C) 2012  Albert van Dalen
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

TFT 240x320 library:
https://www.instructables.com/id/Arduino-TFT-display-and-font-library/
https://github.com/Bodmer/TFT_ILI9341 m

Version 5.2 1-02-2015
Version 6.0 2-01-2017 added menu with TFT display
Version 6.1 22-05-2017 added set display orientation
Version 6.2 26-8-2017 only supports PCB with LCD now, weldContinuously with sinusMax, displayDot improved
Version 7.0 2-10-2017 only for smd board and ILI9341 240x320 TFT display

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
#include <TFT_ILI9341.h> // // https://github.com/Bodmer/TFT_ILI9341 disadvantage: settings in library

#include <Albert.h>
#include <Switch.h>  
#include <EEPROMex.h>
#include "Definitions.h"
#include "Classes.h"
#include "Eeprom.h"

String version = "Version 7.1";

Switch weldButton(weldButtonPin);
Switch upButton(upButtonPin);
Switch downButton(downButtonPin);
Switch selectButton(selectButtonPin);
Switch foodSwitch(foodSwitchPin);

TFT_ILI9341 tft = TFT_ILI9341();  // pins defined in User_Setup.h
UpDownValue WeldItemNr = UpDownValue(0, 1, 0, 2); // 3 items 0 1 2 

Menu menu; 
Eeprom eeprom;
// Set the menu item DEFAULT values: value, step, minValue, maxValue
MenuItem preweldTimeItem = MenuItem("Preweld, ms", UpDownValue(50, 50, 0, 1000)); 
MenuItem pauseTimeItem = MenuItem("Pause, ms", UpDownValue(500, 50, 0, 1000)); 
MenuItem weldTimeItem = MenuItem("Weld time, ms", UpDownValue(250, 50, 0, 1000));
MenuItem menuItems[] = {preweldTimeItem, pauseTimeItem, weldTimeItem};

bool continuously;
unsigned orientation_addr, orientation=3; 

void setup() 
{ Serial.begin(9600);
  setpinModes();
  pollAll(); 
  blinkLed(ledPin, 3); // is a delay too 
  pollAll(); // do after the blinkLed() delay
  eeprom.init();
  eeprom.read(); // set the menu with the stored EEPROM values
  selectTFTorientation(); // do after eeprom.init and before TFTinit()
  selectContinuously();  
  menu.TFTinit();  
  printValuesToSerial();
  menu.displayStart(); 
}
  
void loop() 
{ pollAll();
  menu.control();
  weldControl();
  // test();
}
