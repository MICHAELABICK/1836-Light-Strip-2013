#include "Arduino.h"
#include "RedCycle.h"

RedCycle::RedCycle(int rate, int wait)  {
  red_count = 0;
  _rate = rate
  _wait = wait;
}

void RedCycle::Initialize()  {
  uint16_t i;
  
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, redWheel( ((i * 384 / strip.numPixels()) + red_count) % 384) );
  }  
  strip.show();   // write all the pixels out
  
  red_count += _rate;
  
  delay(_wait);
}

void RedCycle::Run()  {
  return true;
}
