void weldControl()
{ if(continuously) weldContinuously();
  else if(weldButton.pushed()) weldCyclus(menuItems[2].upDownVal); 
}
  
void weldCyclus(int weldTime_ms)
{ pulseWeld(menuItems[0].upDownVal);
  delay(menuItems[1].upDownVal);
  pulseWeld(weldTime_ms);
}

void weldContinuously()
{ if(weldButton.pushed()) sinusMax(); // do only once
  weld(weldButton.on());
}

void pulseWeld(int ms)
{ sinusMax();
  weld(1);
  delay(ms); 
  weld(0);
  if(printValuesToSerial) Serial << ms << endl;  
}

void weld(bool b) 
{ menu.displayDot(b);
  digitalWrite(weldPin, b);
  if(b)Serial << "w "; 
}

void sinusMax()
{ Serial << "sinusMax "; 
  if(sinMaxDisabled) return;
  while(digitalRead(zeroCrossPin));
  while(!digitalRead(zeroCrossPin));
  delayMicroseconds(sinusMax_us); // prevent high inrush current, turn-on at the sinus max 
}

void setpinModes()
{ pinMode(weldButtonPin, INPUT_PULLUP);   
  pinMode(weldPin, OUTPUT);
  pinMode(zeroCrossPin, INPUT);
}

void pollAll()
{ weldButton.poll();
  upButton.poll();
  downButton.poll();
  selectButton.poll();
}

void selectContinuously()
{ const unsigned long switchStable_ms = 200; // min = 50
  const unsigned long weldButtonPressTime = 500; // push weldButton during 500ms at power up
  unsigned long start_ms = millis();
  pollAll();
  delay(switchStable_ms);
  
  while(weldButton.on())
  { pollAll();     
    if(!weldButton.on()) break;
  }
  if(millis()-start_ms > weldButtonPressTime) continuously = 1;
}

void TFTinit()
{ tft.begin();
  tft.setOrientation(eeprom.readInt(orientation)); 
  tft.setFont(Terminal12x16);
}

void printValuesToSerial()
{  Serial << "\npreweld " << menuItems[0].upDownVal << "ms, pause " << menuItems[1].upDownVal << "ms, weldTime " 
   << menuItems[2].upDownVal << "ms, continuously " << continuously << endl;
}

void setOrientation()
{ pollAll(); // works without too
  if(upButton.on() && downButton.on()) 
  { if(eeprom.readInt(orientation)==0) eeprom.writeInt(orientation, 2); // LCD Orientation can only be either 0 or 2
    else eeprom.writeInt(orientation, 0); 
  }
}


