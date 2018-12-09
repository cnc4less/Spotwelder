#include "avdweb_VirtualDelay.h"
VirtualDelay testDelay; // default = millis

void test()
{ testDelay.start(10000); // weld every 10s
  if(testDelay.elapsed()) weldCyclus(menuItems[2].upDownVal); 
}
