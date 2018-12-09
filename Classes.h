#ifndef _Classes_H_
#define _Classes_H_

#include <Arduino.h>

class UpDownValue
{ public:
    UpDownValue(int value, int step, int minValue, int maxValue);
    int up();
    int down();
    UpDownValue::operator int();
   
    int value, step, minValue, maxValue;
};

// ---------------------------------------------------------------------------

class MenuItem
{ public:
    MenuItem(String name, UpDownValue value);
        
    String name;
    UpDownValue upDownVal; 
};

// ---------------------------------------------------------------------------

class Menu
{ public:
    void displayStart();
    void control();
    void displayDot(bool on);
    void TFTinit();
     
  private:
    void displayName(int itemNr, String &name);
    void displayValue(int itemNr, int value, bool focus, bool edit);
    void drawColorTextLine(int line, int left, String str, uint16_t textColor=TFT_WHITE, uint16_t backgroundColor=TFT_BLACK); // line 0..8
    void displayContinuously();

    bool editValueMode=0;
    const int textLeft = 20, fontSize=4, fontHeight=26; 
    const int vertOffset=3; // 240-9*26)/2
};

#endif
