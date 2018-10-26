#include "Arduino.h"
#include "Ring.h"

// layer order

Ring::Ring(int i, int start, int end) {
  index = i;
  length = abs(start - end);
  firstLed = start;
  lastLed = end;

}

void Ring::NoteOn(byte note, byte velocity) {
  // keep a stack of 5 note structs that include note and velocity, added on NoteOn, removed on NoteOff
  //  int noteArray[5];

  int c = ((int)note % 12) * (255.0 / 12.0);
  int brightness = (int)velocity * 2;
  bright = brightness;
  noteIsOn = true;
  lastHit = millis();
  pattern = (int)(((int)note - 10) / 12) - 1;
  color = c;
  //  Serial.print("NOTE: ");
  //  Serial.println(note);
  //  Serial.print("PATTERN: ");
  //  Serial.println(pattern);
  //  Serial.println("=========");
}

void Ring::NoteOff() {
  noteIsOn = false;
}

void Ring::Update() {
  if (bright > 0 && !noteIsOn) {
    int rate = 5;
    if (bright - rate < 0) {
      bright = 0;
    }
    else {
      bright -= rate;
    }
  }
}

struct LED {
  int color; //array for each led  int color[length]
  int bright; //array for each led
  int sat; //array for each led
};

struct LED Ring::GetColor(int i) {
  struct LED led;
  switch (pattern) {
    case 0: // solid
      led.color = color;
      led.bright = bright;
      return led;
    case 1: // all rings their own color
      led.color = (int)(color + index * (255.0 / 3.0));
      led.bright = bright;
      return led;
    case 2: // random sparkle
      led.color = color;
      led.bright = bright;
      if (random(0, 112) == i) {
        led.bright = bright;
      } else {
        led.bright = 0;
      }
      return led;
    case 3: //rainbox: millis keeps counting up forever! change this
      led.color = color + (i - firstLed) * (155.0 / length) * (millis()-lastHit) / 1000;
      led.bright = bright;
      if (index != 0) {
        noteIsOn = false;
      }
      return led;
    case 4: // spiral
      led.color = color + sin(millis()/250.0) * 30;
      if((millis() - lastHit) / 20.0 > i && (millis() - lastHit) / 20.0 - 20 < i){
        led.bright = bright;
      } else {
        led.bright = 0;
      }
      return led;
    case 5: // breathe
      led.color = color + sin(millis()/250.0) * 30;
      led.bright = bright;
      return led;
    case 6: // wave
      led.color = color + 18.0 * sin((i - firstLed) * 100.0 / length * (millis()-lastHit));
      led.bright = bright;
      return led;
    case 7: // inner circle only
      led.color = color + sin(millis()/50.0) * 30; 
      if(index==2){
        led.bright = bright;
      } else {
        led.bright = 0;
      }
      return led;
    case 8: // strobe
      led.color = color + sin(millis()/250.0) * 30;
      led.bright = (millis()%50)%2==0 ? bright : 0;
      return led;
    default:
      // Serial.println("DEFAULT=========");
      // Serial.print("COLOR:");
      // Serial.println(color);
      // Serial.println("=========");
      led.color = color;
      led.bright = bright;
      return led;
  }
}
