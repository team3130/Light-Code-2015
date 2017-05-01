#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    4
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         145
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { chaseRainbow, confetti, sinelon, juggle, bpm, chaseBlackYellow, flashRainbow,
randomHSVSpin, randomHSVChase, spinningRainbow};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int currLED = 1;

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( 20 ) {
    nextPattern();  // change patterns periodically
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos1 = beatsin16(10, 0, NUM_LEDS / 3);
  leds[pos1] += CHSV( gHue, 255, 192);
  leds[pos1 + NUM_LEDS/3] += CHSV( gHue, 255, 192);
  leds[pos1 + NUM_LEDS*2/3] += CHSV( gHue, 255, 192);  
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void spinningRainbow(){
  for(int i = NUM_LEDS - 1; i >= 0; i--){
    leds[i] = leds[i-1];
  }
  leds[0] = CHSV( gHue, 255, 192 );
}

void flashRainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(gHue, 255, BRIGHTNESS);
  }
}

void chaseRainbow() {
  chaser(gHue);
}

void chaseBlackYellow() {
  (gHue % 2 == 0) ? chaser(CRGB::Yellow) : chaser(CRGB::Black);
}

void chaser(CRGB color) {
  fadeToBlackBy( leds, NUM_LEDS, 40);
  leds[(int)(currLED / 2.0)] = color;
  currLED++;
  if (currLED >= NUM_LEDS * 2) {
    currLED = 0;
  }
  FastLED.delay(50);
}

void randomHSVChase(){
fadeToBlackBy( leds, NUM_LEDS, 40);
  leds[(int)(currLED/2.0)] = CHSV( random8(), 255, BRIGHTNESS);
  currLED++;
  if(currLED>=NUM_LEDS*2){currLED=0;}
  FastLED.delay(50);
}

void randomHSVSpin() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = leds[i - 1];
  }
  leds[0] = CHSV( random8(), 255, BRIGHTNESS );
  FastLED.delay(20);
}

