#include "LPD8806.h"
#include "SPI.h"

#include <RedCycle.h>

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 22;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 4;
int clockPin = 5;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

//Setup the RedCycle object
  RedCycle redCycle(128/100, 0)

//setup stuff for longshoot loop
uint16_t wait = 500;
int maxshots = 3;  //there is only allowed to be 3 shots on the strip at any one time
int shotcount[3];  //this is a counter to keep track of the places the shots are
int arraycount = 0;  //this makes sure that the shotcount array does not go past it's boundary

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  
  for(int i = 0; i < maxshots; i++) {
    shotcount[i] = strip.numPixels() + 1;
  }
}


void loop() {
  int shootlen = 8;
  
  //THIS IS A DEMO CYCLE
  //rainbowCycle(0);  // make it go through the cycle fairly fast
  
  //Run the redCycle loop 6 times
  for(i = 0; i < 384 * 6; i++) {
    redCycle.initialize();
  }
  
 
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  strip.show();
  
  delay(1000);
  
  wheelspinup(15, shootlen);
  
  delay(1500);
  
  for(int i = 0; i < 6; i++) shoot(8, shootlen);
  
  
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  strip.show();
  
  delay(1000);
  
  wait = 500;
  for(int i = 0; i < maxshots; i++) {
    shotcount[i] = strip.numPixels() + 1;
  }
  
  boolean at_speed = true;
  boolean is_shoot = false;
  longshootCycle(at_speed, is_shoot);
  
  delay(1500);
  
  is_shoot = true;
  at_speed = true;
  for(int i = 0; i < 300; i++) longshootCycle(at_speed, is_shoot);
}

void longshootCycle(boolean at_speed, boolean is_shoot) {  
  if(is_shoot == true & wait >= 65) {  //wait to shoot after the last shot 
    shotcount[arraycount] = 0;
    wait = 0;
    arraycount++;
  }
  
  if(arraycount >= maxshots - 1) {  //reset arraycount if it reaches maxhot so that we don't go over array size
    arraycount = 0;
  }
  
  if(at_speed == true & wait >= 50) {  //if the wheel is up to speed and you have waited since the last shot
    for (int i=0; i < 3; i++) {
      strip.setPixelColor(i, strip.Color(127, 60 - (20 * i), 0));
    }
    for (int i=3; i < 5; i++) {
      strip.setPixelColor(i, strip.Color(127, 0, 0));
    }
    for (int i=5; i < 6; i++) {
      strip.setPixelColor(i, strip.Color(50, 50, 50));
    }
  }
  
  for(int k = 0; k < maxshots; k++) {
    if(shotcount[k] < strip.numPixels() + 1) {
      int j = shotcount[k];
      
      for (int i=j; i < j + 3; i++) {
        strip.setPixelColor(i, strip.Color(127, 60 - (20 * (i - j)), 0));
      }
      for (int i=j + 3; i < j + 5; i++) {
        strip.setPixelColor(i, strip.Color(127, 0, 0));
      }
      for (int i=j + 5; i < j + 6; i++) {
        strip.setPixelColor(i, strip.Color(5, 5, 5));
      }
      
      strip.setPixelColor(j - 1, 0);  //clear the back end
      
      shotcount[k]++;  //iterate shotcount
    }
  }
  
  strip.show();
    
  wait++;
  
  delay(4);  //delay should be 4 milliseconds
}

void wheelspinup(uint8_t wait, uint8_t len) {
  int i;
  
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  
  for(i=0; i < len - 4; i++) {
    strip.setPixelColor(i, strip.Color(100, 70, 70));
    strip.show();
    delay(wait);
  }
  
  for(i=len - 4; i < len - 3; i++) {
    strip.setPixelColor(i, strip.Color(127, 100, 0));
    strip.show();
    delay(wait);
  }
  
  for(i=len - 3; i < len - 2; i++) {
    strip.setPixelColor(i, strip.Color(127, 25, 0));
    strip.show();
    delay(wait);
  }
  
  for(i=len - 2; i < len; i++) {
    strip.setPixelColor(i, strip.Color(127, 0, 0));
    strip.show();
    delay(wait);
  }
}

void shoot(uint8_t wait, uint8_t len) {
 int j, i;
 
 for (j= len - 2; j <= strip.numPixels(); j++) {
   for (i=j; i < j + 2; i++) {
     strip.setPixelColor(i, strip.Color(127, 0, 0));
   }   
   strip.show();
   
   strip.setPixelColor(j, 0);
   
   delay(wait);
 }
 
 delay(150);
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t redWheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = (127 - r)/70;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 0;  //green down
      b = 0;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      r = WheelPos % 128;      //red up
      b = (127 - r)/70;  //blue down 
      g = 0;                  //green off
      break;
  }
  return(strip.Color(r,g,b));
}

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
