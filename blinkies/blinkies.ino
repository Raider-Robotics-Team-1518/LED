/*
 * blinkies - robot controlled LEDs for making our bot blinky
 * 
 * Uses the WS2812B RGB pixel LEDs. It shows random color palettes while the bot is not active, then the alliance color during the game. 
 *
 * RoboRio sends high/low signals to inputs 3 & 4 on the arduino.
 * Code below switches on those pin states:
 *      * If both high/on - random palettes
 *      * If one high - all solid red
 *      * If the other high - all solid blue
 *      * If both low/off - all off / black 
 * 
*/

#include "FastLED.h"

// UPDATE THESE VALUES TO MATCH YOUR SETUP
// 
// define pins that receive high/low values from Rio
int inputPin5 = 5;
int inputPin4 = 4;
// Data pin that led data will be written out to the LEDs over
#define DATA_PIN 3
// How many leds are in the strip?
#define NUM_LEDS 96
// don't go above about 60 if you're powering off the power 5v
// lead on the power distr. panel instead of USB
#define BRIGHTNESS  200        // range of 0 - 255 
#define LED_TYPE    WS2812B    // our pixel strip type

// THE REMAINDER CAN PROBABLY LEFT AS-IS
// 
// constants for solid colors
int BLACK = 0;
int BLUE = 1;
int RED = 2;
int GREEN = 3;

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

#define COLOR_ORDER GRB        // color order - green/red/blue
#define UPDATES_PER_SECOND 250 // controls update frequency

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
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    // set up our input pins on the arduino
    pinMode(inputPin5, INPUT);
    pinMode(inputPin4, INPUT);
    digitalWrite(inputPin5, HIGH);
    digitalWrite(inputPin4, HIGH);
    // set our starting palette
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

// this function is called repeatedly, every NN milliseconds
void loop() {
    ChangePalettePeriodically();

    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; 
    // reads from the switch pins and makes sure they are
    // a 1 or 0 in the 
    int reading5 = 1 - digitalRead(inputPin5);
    int reading4 = 1 - digitalRead(inputPin4);

    if (reading5 == 1 && reading4 == 1) {
        FillLEDsFromPaletteColors( startIndex);
    } else if (reading5 == 0 && reading4 == 1) {
        turnColor(RED);
    } else if (reading5 == 1 && reading4 == 0) {
        turnColor(BLUE);
    } else if(reading5 == 0 && reading4 == 0) {
        turnColor(GREEN);
    }
    FastLED.show();
    delay(1000 / UPDATES_PER_SECOND);
}

void turnColor(int color) {
  CRGB clr;
  if (color == BLUE) {
    clr = CRGB::Blue;
  } else if (color == RED) {
    clr = CRGB::Red;
  } else if (color == GREEN) {
    clr = CRGB::Green;
  } else {
    clr = CRGB::Black;
  }
  for (int led = 0; led < NUM_LEDS; led = led + 1) {
    leds[led] = clr;
  }
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
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
        currentPalette[i] = CHSV( random8(), 255, random8());
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
