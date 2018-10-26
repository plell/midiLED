#ifndef Ring_h
#define Ring_h
#include "Arduino.h"  

class Ring {
  public:
    Ring(int index, int start, int end);
    void Update();
    struct LED GetColor(int i);
    void NoteOn(byte note, byte velocity);
    void NoteOff();
    int length;
    int index;
    int bright = 0;

    int lastHit;
    
    int firstLed;
    int lastLed;
    
    int color;
    int sat;
    int gradient;
    int noteIsOn;
    int pattern;
};

#endif
