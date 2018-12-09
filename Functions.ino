void selectTFTorientation() // because LCD orientation is unknown
{ if(upButton.on() && downButton.on()) // change between 1<->3 when up and down are pressed during power up
  { if(orientation==1) orientation=3; // orientation can only be either 1 or 3
    else orientation=1; // if 0, 2, 3...
    eeprom.write();
  }
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

void weldControl()
{ if(continuously) weldContinuously();
  else if(weldButton.pushed() || foodSwitch.pushed()) weldCyclus(menuItems[2].upDownVal); 
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
  if(ms) weld(1); // avoid short displayDot flash
  delay(ms); 
  weld(0);
  Serial << ms << endl; 
}

void weld(bool b) 
{ menu.displayDot(b);
  digitalWrite(weldPin, b);
  if(b) Serial << "w "; 
}

void sinusMax()
{ Serial << "sinusMax "; 
  if(sinMaxDisabled) return;
  while(digitalRead(zeroCrossPin));
  while(!digitalRead(zeroCrossPin));
  delayMicroseconds(sinusMax_us); // prevent high inrush current, turn-on at the sinus max 
}

void setpinModes()
{ pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(weldButtonPin, INPUT_PULLUP);
  pinMode(weldPin, OUTPUT);
  pinMode(ledPin, OUTPUT); // is also done in blinkLed
  pinMode(zeroCrossPin, INPUT);
  pinMode(foodSwitchPin, INPUT_PULLUP);
}

void pollAll()
{ weldButton.poll();
  upButton.poll();
  downButton.poll();
  selectButton.poll();
  foodSwitch.poll();
}

void printValuesToSerial()
{  Serial << "\npreweld " << menuItems[0].upDownVal << "ms, pause " << menuItems[1].upDownVal << "ms, weldTime " 
   << menuItems[2].upDownVal << "ms, continuously " << continuously << ", Orientation " << orientation << endl;
}



