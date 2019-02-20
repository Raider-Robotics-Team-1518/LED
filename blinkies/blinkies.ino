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
#define matchStartedPin 6
#define targetInViewPin 7
#define linedUpPin 8
#define alliancePin 9
// Data pin that led data will be written out to the LEDs over

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

// THE REMAINDER CAN PROBABLY LEFT AS-IS
// 
// constants for solid colors
#define BLACK 0
#define BLUE 1
#define RED 2
#define YELLOW 3
#define GREEN 4

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
    
    pinMode(matchStartedPin, INPUT);
    pinMode(targetInViewPin, INPUT);
    pinMode(linedUpPin, INPUT);
    pinMode(alliancePin, INPUT);
    
    digitalWrite(matchStartedPin, LOW);
    digitalWrite(targetInViewPin, LOW);
    digitalWrite(linedUpPin, LOW);
    digitalWrite(alliancePin, LOW);
    
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
    bool isMatchStarted = (1 - digitalRead(matchStartedPin)) == 0;
    bool isTargetInView = (1 - digitalRead(targetInViewPin)) == 0;
    bool isLinedUp = (1 - digitalRead(linedUpPin)) == 0;
    bool isAllianceColorRed = (1 - digitalRead(alliancePin)) == 0;

    isMatchStarted = false;
    isTargetInView = true;
    isLinedUp = false;
    isAllianceColorRed = true;
    if (isMatchStarted) {
        if (isTargetInView) {
          if (isLinedUp) {
            turnColor(GREEN);
          } else {
            turnColor(YELLOW);
          }
        } else {
          if (isAllianceColorRed) {
            turnColor(RED);
          } else {
            turnColor(BLUE);
          }
        }
    } else {
        FillLEDsFromPaletteColors( startIndex);
    }
    FastLED.show();
    delay(1000 / UPDATES_PER_SECOND);
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
