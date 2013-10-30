#ifndef RedCycle_h
#define RedCycle_h

#include "Arduino.h"

class RedCycle
{
  public:
    void Initialize();
    void Run();
  private:
    uint32_t redWheel();
    int red_count;
    int _rate;
}

#endif
