/*
 * blinkies - robot controlled LEDs for making our bot blinky
 * 
*/

#include "FastLED.h"

#define LEFT_UPRIGHT_PIN 3
#define RECTANGLE_PIN 4
#define RIGHT_UPRIGHT_PIN 5

#define NUM_LEDS_LEFT 27
#define NUM_LEDS_RIGHT 27
#define NUM_LEDS_RECTANGLE 42

#define BRIGHTNESS 100
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define UPDATES_PER_SECOND 200

CRGB left_leds[NUM_LEDS_LEFT];
CRGB right_leds[NUM_LEDS_RIGHT];
CRGB rectangle_leds[NUM_LEDS_RECTANGLE];

// 
// constants for solid colors
#define BLACK 0
#define BLUE 2
#define RED 1
#define YELLOW 3
#define GREEN 4
int currentColor = BLACK;

// variables hold some palette info for later use
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


// This function is run once when the program starts. It sets up
// the led, sand tells the controller about them
void setup() {
    delay(2000); // delay for 2 sec while the arduino powers up
    // set up the FastLED library to write to our configuration of LEDs
    FastLED.addLeds<CHIPSET, LEFT_UPRIGHT_PIN, COLOR_ORDER>(left_leds, NUM_LEDS_LEFT).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<CHIPSET, RIGHT_UPRIGHT_PIN, COLOR_ORDER>(right_leds, NUM_LEDS_RIGHT).setCorrection( TypicalLEDStrip );
    FastLED.addLeds<CHIPSET, RECTANGLE_PIN, COLOR_ORDER>(rectangle_leds, NUM_LEDS_RECTANGLE).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    Serial.begin(9600);
    
    
    // set our starting palette
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

// this function is called repeatedly, every NN milliseconds
void loop() {
    ChangePalettePeriodically();

    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; 


    int num = int(Serial.read() - '0');
    switch (num) {
      case 0:
        currentColor = BLACK;
        break;
      case 1:
        currentColor = RED;
        break;
      case 2:
        currentColor = BLUE;
        break;
      case 3:
        currentColor = YELLOW;
        break;
      case 4:
        currentColor = GREEN;
        break;
      case 5:
        currentColor = 5;
        break;
      default:
        currentColor = currentColor;
   }

    if (currentColor < 5){
      turnColor(currentColor);
      
    } else {
      FillLEDsFromPaletteColors(startIndex);
    } 
    FastLED.show();
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
  for (int i = 0; i < NUM_LEDS_LEFT; i++) {
    left_leds[i] = clr;
    right_leds[i] = clr;
    }
  for (int i = 0; i < NUM_LEDS_RECTANGLE; i++) {
    rectangle_leds[i] = clr;
  }
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for (int i = 0; i < NUM_LEDS_LEFT; i++) {
        left_leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        right_leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    }
    for (int i = 0; i < NUM_LEDS_RECTANGLE; i++) {
        rectangle_leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    }

    colorIndex += 3;
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;       currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { currentPalette = PartyColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();           currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { currentPalette = RainbowColors_p;       currentBlending = NOBLEND; }
        if( secondHand == 35)  { currentPalette = RainbowColors_p;       currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), random8(), random8());
    }
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
