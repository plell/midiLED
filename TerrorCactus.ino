#include "FastLED.h"
#include "Ring.h"

#define NUM_LEDS 112
#define DATA_PIN 2
#define OUTER_START 0
#define INNER_START 52
#define CIRCLE_START 96
#define FRAMES_PER_SECOND 120

// 52 LEDS in the outside
// 44 LEDS in the inside
// 16 LEDS in the circle

Ring outerRing(0, OUTER_START, INNER_START);
Ring innerRing(1, INNER_START, CIRCLE_START);
Ring circle(2, CIRCLE_START, NUM_LEDS);

Ring rings[3] = {
  outerRing, innerRing, circle
};

int is[NUM_LEDS] = {
  0, 1, 2, 3, 4, 5, 6, 94, 93, 92, 91, 86,
  85, 84, 83, 78, 77, 72, 71, 70, 69, 64, 63, 62,
  61, 53, 54, 55, 56, 57, 58, 59, 48, 47, 46, 45,
  40, 39, 38, 37, 32, 31, 28, 27, 19, 20, 21, 22,
  12, 13, 14, 11,

  10, 9, 8, 7, 95, 88, 89, 90, 87, 80, 81, 82,
  79, 76, 73, 74, 75, 68, 65, 66, 67, 60, 52, 51,
  50, 49, 44, 43, 42, 41, 36, 35, 34, 33, 30, 29,
  26, 25, 24, 23, 18, 17, 16, 15,

  104, 103, 102, 101, 100, 99, 98, 97, 96, 111, 110,
  109, 108, 107, 106, 105
};

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.print(note, velocity);
  for (int i = 0; i < 3; i++) {
    rings[i].NoteOn(note, velocity);
  }
  Serial.print("NOTE ON");
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  Serial.println("NOTE OFF");
  for (int i = 0; i < 3; i++) {
    rings[i].NoteOff();
  }
}

struct LED {
  int color;
  int bright;
  int sat;
};


void loop() {
  usbMIDI.read();
  for (int i = 0; i < NUM_LEDS; i++) {
    for (int r = 0; r < 3; r++) {
      if (i >= rings[r].firstLed && i < rings[r].lastLed) {
        LED led;
        led = rings[r].GetColor(i);
        leds[is[i]] = CHSV(led.color, 255, led.bright);
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    rings[i].Update();
  }

  FastLED.show();
  FastLED.delay(1);

}
