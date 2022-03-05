#include <FastLED.h>

#define NUM_LEDS 6
#define DATA_PIN 2
#define COLOR_ORDER GRB

CRGB realleds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
CRGBSet left_side_leds(leds(0, 2));
CRGBSet right_side_leds(leds(3, 5));
uint16_t LEDS_PER_SIDE = 3;

// constants for solid colors
#define BLACK 0
#define BLUE 2
#define RED 1
#define YELLOW 3
#define GREEN 4
#define RAINBOW 5

int currentColor = BLACK;
int flash_state = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(84);

}

void turnColor(int color) {
  CRGB clr;
  switch(color) {
    case BLUE:
        clr = CRGB::Blue;
        break;
    case RED:
        clr = CRGB::Red;
        break;
    case YELLOW:
        clr = CRGB::Yellow;
        break;
    case GREEN:
        clr = CRGB::Green;
        break;
    default:
        clr = CRGB::Black;
        break;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = clr;
  }
}

void flash_color(int color) {
  if (flash_state == 0) {
    turnColor(color);
    flash_state = 1;
  } else {
    turnColor(BLACK);
    flash_state = 0;
  }
  FastLED.show();
}

void alternate_side_colors() {
  if (flash_state == 0) {
    flash_state = 1;
    for(int i=0; i<LEDS_PER_SIDE; i++) {
      left_side_leds[i] = CRGB::Red;
      right_side_leds[i] = CRGB::Blue;
    }
  } else {
    flash_state = 0;
    for(int i=0; i<LEDS_PER_SIDE; i++) {
      left_side_leds[i] = CRGB::Blue;
      right_side_leds[i] = CRGB::Red;
    }
  }
  FastLED.show();
}

void loop() {
  EVERY_N_MILLISECONDS(300) {
//    flash_color(RED);
    alternate_side_colors();
  }
}
