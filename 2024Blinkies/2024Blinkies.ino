#include <FastLED.h>

// UPDATE THESE
#define NUM_LEDS 32
#define LEDS_PER_SIDE 16
#define DATA_PIN 2
#define COLOR_ORDER GRB
#define BRIGHTNESS 156

// define the pin numbers for the DIO input "switches"
#define INPUT_PIN_1 6
#define INPUT_PIN_2 7
#define INPUT_PIN_3 8
#define INPUT_PIN_4 9

// constants for solid colors
/** from Robot code:
    OFF(false, false, false), // 0
    RED(true, false, false), // 1
    BLUE(false, true, false), // 2
    GREEN(true, true, false), // 3
    ORANGE(false, false, true), // 4
    YELLOW(true, false, true), // 5
    PURPLE(false, true, true), // 6
    DEFAULT(true, true, true); // 7
*/
#define BLACK 0
#define RED 1
#define BLUE 2
#define GREEN 3
#define ORANGE 4
#define YELLOW 5
#define PURPLE 6
#define RAINBOW 7

// constants for the light sequences to show
#define ALL_OFF 0
#define OFF 0
#define ALL_RED 1
#define ALL_BLUE 2
#define FLASHING_RED 3
#define FLASHING_BLUE 4
// not sure what to call these, so first is where all are red and
// then the blue "fills up" the strip and reverses "draining back out"
// the second here is the opposite
#define RED_WITH_BLUE_STRIPE 5
#define BLUE_WITH_RED_STRIPE 6

CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType currentBlending = LINEARBLEND;

// you shouldn't need to modify these variables
CRGB realleds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
CRGBSet left_side_leds(leds(LEDS_PER_SIDE - 1, 0));
CRGBSet right_side_leds(leds(LEDS_PER_SIDE, NUM_LEDS - 1));
int currentColor = BLACK;
int flash_state = 0;
int current_pos = 0;
int delta = 0;

void setup() {
  pinMode(INPUT_PIN_1, INPUT);
  pinMode(INPUT_PIN_2, INPUT);
  pinMode(INPUT_PIN_3, INPUT);
  pinMode(INPUT_PIN_4, INPUT);
  digitalWrite(INPUT_PIN_1, HIGH);
  digitalWrite(INPUT_PIN_2, HIGH);
  digitalWrite(INPUT_PIN_3, HIGH);
  digitalWrite(INPUT_PIN_4, HIGH);

  Serial.begin(115200);
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

CRGB get_color_from_name(int color) {
  /** from Robot code:
      OFF(false, false, false), // 0
      RED(true, false, false), // 1
      BLUE(false, true, false), // 2
      GREEN(true, true, false), // 3
      ORANGE(false, false, true), // 4
      YELLOW(true, false, true), // 5
      PURPLE(false, true, true), // 6
      RAINBOW(true, true, true); // 7
  */
  CRGB clr;
  switch (color) {
    case RED:
      clr = CRGB::Red;
      break;
    case BLUE:
      clr = CRGB::Blue;
      break;
    case GREEN:
      clr = CRGB::Green;
      break;
    case ORANGE:
      clr = CRGB::Orange;
      break;
    case YELLOW:
      clr = CRGB::Yellow;
      break;
    case PURPLE:
      clr = CRGB::Purple;
      break;
    case OFF:
    case ALL_OFF:
    default:
      clr = CRGB::Black;
      break;
  }
  return clr;
}
void set_solid_color(int color) {
  /*
  Set all LEDs to a single color. You should NOT call FastLED.show() after
  calling this function.

  Argument:
    color   (int) One of the defined color constants, eg. BLUE
  */
  FastLED.showColor(get_color_from_name(color));
}

void flash_color(int color) {
  /*
  Alternates setting all LEDs to a single color or to black with each call.
  If called rapidly enough (~100 ms) then the LEDs appear to "flash" the given
  color.

  Argument:
    color   (int) One of the defined color constants, eg. BLUE
  */
  if (flash_state == 0) {
    set_solid_color(color);
    flash_state = 1;
  } else {
    set_solid_color(BLACK);
    flash_state = 0;
  }
}

void alternate_side_colors(int foreground_color, int background_color) {
  /*
  Alternates the left and right side LEDs between the red/blue alliance
  colors.
  */
  static CRGB color1, color2;
  if (flash_state == 0) {
    flash_state = 1;
    color1 = get_color_from_name(foreground_color);
    color2 = get_color_from_name(background_color);
  } else {
    flash_state = 0;
    color1 = get_color_from_name(background_color);
    color2 = get_color_from_name(foreground_color);
  }
  for (int i = 0; i < LEDS_PER_SIDE; i++) {
    left_side_leds[i] = color1;
    right_side_leds[i] = color2;
  }
  FastLED.show();
}

void grow_shrink_color(int foreground_color, int background_color) {
  /*
  Start with all LEDs set to background_color. Then, one-by-one, the LEDs
  are set to the foreground_color. Once all are foreground_color, the
  direction switches and one-by-one, the LEDs go back to being the
  background_color.

  Argument:
    foreground_color   (int) One of the defined color constants, eg. BLUE
    background_color   (int) One of the defined color constants, eg. RED
  */
  for (int i = 0; i < LEDS_PER_SIDE; i++) {
    if (i <= current_pos) {
      left_side_leds[i] = get_color_from_name(foreground_color);
      right_side_leds[i] = get_color_from_name(foreground_color);
    } else {
      left_side_leds[i] = get_color_from_name(background_color);
      right_side_leds[i] = get_color_from_name(background_color);
    }
  }
  FastLED.show();

  if (current_pos == LEDS_PER_SIDE - 1) {
    // All LEDs show foreground_color, so reverse directions
    delta = -1;
  } else if (current_pos == 0) {
    // At the start, all are background color
    delta = 1;
  }
  current_pos = (current_pos + delta + LEDS_PER_SIDE) % LEDS_PER_SIDE;
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (int j = 0; j < LEDS_IN_BASE; j++) {
    if (j < LEDS_PER_TOWER) {
      tower_1_leds[j] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS,
                                         currentBlending);
      tower_2_leds[j] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS,
                                         currentBlending);
    }
    base_leds[j] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS,
                                    currentBlending);
    colorIndex += 3;
  }
}
void animatePalette() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);
  FastLED.show();
}

int get_selected_pattern_from_rio() {
  // the "1 -" part below converts a 1 into a 0 and 0 into a 1
  // which seems odd, but that makes our logic here match what's
  // set in the Java code
  int dio_pin_1 = (digitalRead(INPUT_PIN_1));
  int dio_pin_2 = (digitalRead(INPUT_PIN_2)) * 2;
  int dio_pin_3 = (digitalRead(INPUT_PIN_3)) * 4;
  int dio_pin_4 = (digitalRead(INPUT_PIN_4)) * 8;

  /*
   * The multiplication let's us easily figure out which combination of
   * switches are on vs off.
   *   All switches off = 0 + 0 + 0 + 0 = 0
   *   All switches on = 1 + 2 + 4 + 8 = 15
   *   Switch 1 & 2 on, others off = 1 + 2 + 0 + 0 = 3
   *   etc.
   */
  return dio_pin_1 + dio_pin_2 + dio_pin_3 + dio_pin_4;
}

void loop() {
  static int millis_delay = 300;
  static int pattern = 0;
  EVERY_N_MILLISECONDS(10) {
    // poll the rio every 10 milliseconds
    pattern = get_selected_pattern_from_rio();
  }
  /** from Robot code:
      OFF(false, false, false), // 0
      RED(true, false, false), // 1
      BLUE(false, true, false), // 2
      GREEN(true, true, false), // 3
      ORANGE(false, false, true), // 4
      YELLOW(true, false, true), // 5
      PURPLE(false, true, true), // 6
      RAINBOW(true, true, true); // 7
  */
  EVERY_N_MILLIS_I(loop_timer, 200) {
    switch (pattern) {
      case ALL_OFF:
      case OFF:
        set_solid_color(BLACK);
        break;
      case ALL_RED:
        set_solid_color(RED);
        break;
      case ALL_BLUE:
        set_solid_color(BLUE);
        break;
      case GREEN:
        set_solid_color(GREEN);
        break;
      case ORANGE:
        set_solid_color(ORANGE);
        break;
      case YELLOW:
        set_solid_color(YELLOW);
        break;
      case PURPLE:
        set_solid_color(PURPLE);
        break;
      case RAINBOW:
        millis_delay = 1;
        animatePalette();
      case FLASHING_BLUE:
        millis_delay = 166;
        flash_color(BLUE);
        break;
      case RED_WITH_BLUE_STRIPE:
        millis_delay = 50;
        grow_shrink_color(BLUE, RED);
        break;
      case BLUE_WITH_RED_STRIPE:
        millis_delay = 50;
        grow_shrink_color(RED, BLUE);
        break;
      default:
        set_solid_color(BLACK);
    }
    loop_timer.setPeriod(millis_delay);
  }
}
