#include "Classes.h"
#include <Arduino.h>

void Menu::control() 
{ if(!editValueMode)
  { if(upButton.pushed()) 
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 0, editValueMode); // un-focus old item
      displayValue(WeldItemNr, menuItems[WeldItemNr.down()].upDownVal, 1, editValueMode); // focus
      return;
    } 
    if(downButton.pushed()) 
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 0, editValueMode); // un-focus old item
      displayValue(WeldItemNr, menuItems[WeldItemNr.up()].upDownVal, 1, editValueMode); // focus
      return;
    } 
    if(selectButton.pushed())
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 1, editValueMode=1); // add backgound
      return;
    }
  }   
    
  if(editValueMode)
  { if(upButton.pushed()) 
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal.up(), 1, editValueMode); 
      return;     
    }
    if(downButton.pushed()) 
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal.down(), 1, editValueMode); 
      return;    
    } 
    if(selectButton.pushed()) 
    { displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 1, editValueMode=0); // delete backgound
      eeprom.write();
      return;
    }      
  }
}

void Menu::displayStart() 
{ drawColorTextLine(0, textLeft, version, TFT_YELLOW);
  delay(1000);
  TFTinit();
 
  if(!continuously)
  { for(int i=0; i<3; i++)
    { WeldItemNr.value=i;
      displayName(WeldItemNr, menuItems[WeldItemNr].name); 
      displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 0, 0);
    } 
    WeldItemNr.value = 0;
    displayValue(WeldItemNr, menuItems[WeldItemNr].upDownVal, 1, 0); // item 0 has focus
    displayDot(0); 
    drawColorTextLine(8, 0, "   down           up                sel                    ", TFT_YELLOW, TFT_BLUE); // to long text is ok
  }
  else displayContinuously();
}

void Menu::displayContinuously()
{ tft.fillScreen(TFT_RED);
  drawColorTextLine(2, 0, "                 Continuously", TFT_WHITE, TFT_RED);
  drawColorTextLine(3, 0, "                         mode", TFT_WHITE, TFT_RED);
  drawColorTextLine(4, 0, "                    Take care!", TFT_WHITE, TFT_RED);
  displayDot(0);
}

void Menu::displayDot(bool on)
{ tft.fillCircle(148, 180, 20, on? TFT_WHITE : 0x64A7/*green*/); // https://ee-programming-notepad.blogspot.nl/2016/10/16-bit-color-generator-picker.html
}

void Menu::drawColorTextLine(int line, int left, String str, uint16_t textColor, uint16_t backgroundColor) 
{ tft.setTextColor(textColor, backgroundColor);
  tft.drawString(str.c_str(), left, fontHeight*line+vertOffset, fontSize); 
}

void Menu::displayName(int WeldItemNr, String &name) 
{ drawColorTextLine(2*WeldItemNr, textLeft, name, TFT_YELLOW);
}

void Menu::displayValue(int WeldItemNr, int value, bool focus, bool edit) 
{ drawColorTextLine(2*WeldItemNr+1, textLeft, "                  ", TFT_WHITE); // clear line, can't be BLACK (bug?)
  drawColorTextLine(2*WeldItemNr+1, 0, ">", focus? TFT_WHITE : TFT_BLACK); // BLACK on BLACK = BLACK
  drawColorTextLine(2*WeldItemNr+1, textLeft, String(value), edit? TFT_BLACK : TFT_WHITE, edit? TFT_CYAN : TFT_BLACK);
}

void Menu::TFTinit()
{ tft.init();
  tft.setRotation(orientation);
  tft.fillScreen(TFT_BLACK);
}

// ---------------------------------------------------------------------------

UpDownValue::UpDownValue(int value, int step, int minValue, int maxValue):
value(value), step(step), minValue(minValue), maxValue(maxValue)
{
}

int UpDownValue::up() 
{ value += step;       
  return value = (value > maxValue) ?  minValue : value;
}

int UpDownValue::down() 
{ value -= step;       
  return value = (value < minValue ) ?  maxValue : value;
}

UpDownValue::operator int() // conversion operator, object returns value
{ return value; 
} 

// ---------------------------------------------------------------------------

MenuItem::MenuItem(String name, UpDownValue value):
name(name), upDownVal(value)
{  
}

/*
  if(downButton.on() && selectButton.on())
  { preferencesControl();
    return;
  } 
  
  drawColorTextLine(2, textLeft, "Set preferences:", TFT_YELLOW);
  drawColorTextLine(3, textLeft, "press down + sel", TFT_YELLOW);
*/


