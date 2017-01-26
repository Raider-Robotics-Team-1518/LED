#include "FastLED.h"

int inputPin3 = 3;
int inputPin4 = 4;

int WHITE = 0;
int BLUE = 1;
int RED = 2;

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 150

// Data pin that led data will be written out over
#define DATA_PIN 5

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the ledsand tells the controller about them
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(inputPin3, INPUT);
  pinMode(inputPin4, INPUT);
  digitalWrite(inputPin3, HIGH);
  digitalWrite(inputPin4, HIGH);

}

void loop() {
  int reading3 = 1 - digitalRead(inputPin3);
  int reading4 = 1 - digitalRead(inputPin4);
  if (reading3 == 1 && reading4 == 1) {
    turnColor(WHITE);
  } else if (reading3 == 1 && reading4 == 0) {
    turnColor(RED);
  } else if (reading3 == 0 && reading4 == 1) {
    turnColor(BLUE);
  } else {
  }
  delay(1000);

}

void turnColor(int color) {
  for (int led = 0; led < NUM_LEDS; led = led + 1) {
    if (color == BLUE) {
      leds[led] = CRGB::Blue;
    } else if (color == RED) {
      leds[led] = CRGB::Green;
    } else {
      leds[led] = CRGB::White;
    }
    FastLED.show();
  }
}

