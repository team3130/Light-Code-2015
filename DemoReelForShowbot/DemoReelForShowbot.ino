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

#define DATA_PIN1    4
#define DATA_PIN2    5
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS_PER_STRIP    64
CRGB ledsLeft[NUM_LEDS_PER_STRIP];
CRGB ledsRight[NUM_LEDS_PER_STRIP];
//#define BRIGHTNESS          96

boolean dimming = false;
char rxChar = '1';
char preChar = 0;
int colorNum = 0;
int gHue = 0;
int BRIGHTNESS = 96;
int FRAMES_PER_SECOND = 120;

CRGB color[] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Grey, CRGB::White, CRGB::HotPink, CRGB::LemonChiffon, CRGB::DarkKhaki, CRGB::LightSteelBlue};

CRGB colorAmerica[] = {CRGB::Red, CRGB::White,CRGB::Blue};

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(9600);
  Serial.flush();
  printHelp();
  
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN1, COLOR_ORDER>(ledsLeft, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(ledsRight, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


void loop()
{
  if (Serial.available() > 0){
    rxChar = Serial.read();
    Serial.flush();
  }
    
    switch(rxChar){
      
    case '0':                          // If received a ?:
    printHelp();                   // print the command list.
    rxChar = preChar;
    break;
    
    case '1':
    allAsOne(CRGB::Red);
    break;

    case '2':
    allAsOne(CRGB::Green);
    break;
    
    case '3':
    allAsOne(CRGB::Blue);
    break;

    case '4':
    rgbFlash();
    break;

    case '5':
    sine();
    break;

    case '6':
    breathStatic();
    break;

    case '7':
    dim();
    rxChar = preChar;
    break;

    case '8':
    brighten();
    rxChar = preChar;
    break;

    case '9':
    rainFlash();
    break;
    
    case 'a':
    singleColorChange();
    rxChar = 'p';
    break;

    case 'A':
    americaFlash();
    break;
    
    case 'D':
    FRAMES_PER_SECOND = 500;
    death();
    break;

    case 'p':
    pauseLights();
    break;
    
    default:                     
      Serial.print("'");
      Serial.print((char)rxChar);
      Serial.println("' is not a command!");
      rxChar = preChar;
  }
      preChar = rxChar;


      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      if(FRAMES_PER_SECOND > 120){
          FRAMES_PER_SECOND = 120; 
      }

      // do some periodic updates

}

void printHelp(void) {
    Serial.println("--- Command list: ---");
    Serial.println("0 -> Print this HELP");
    Serial.println("1 -> Make LEDS Red");
    Serial.println("2 -> Make LEDS Green");
    Serial.println("3 -> Make LEDS Blue");
    Serial.println("4 -> Make it flash RGB");
    Serial.println("5 -> Make it go to a Sine");
    Serial.println("6 -> Make it Breath one color");
    Serial.println("7 -> Dim");
    Serial.println("8 -> Brighten");
    Serial.println("9 -> Make it flash rainbow");
    Serial.println("a -> Make it cycle one color");
    Serial.println("A -> Make it flash American");
    Serial.println("D -> Make DEATH!!!");
  }


void allAsOne(CRGB color) {
  for(int i = 0; i<NUM_LEDS_PER_STRIP; i++){
    ledsLeft[i] = color;
  }
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void americaFlash(){
    allAsOne(colorAmerica[colorNum%3]);
    colorNum++;
    FastLED.delay(10000 / FRAMES_PER_SECOND);
}

void breathStatic(){
  int preBRIGHTNESS = BRIGHTNESS;
  fill_rainbow(ledsLeft, NUM_LEDS_PER_STRIP, 0);
  BRIGHTNESS = preBRIGHTNESS;
  if(dimming){
    dim();
    if(BRIGHTNESS<15){dimming = false;}
  }else if(!dimming){
    brighten();
    if(BRIGHTNESS>240){dimming = true;}
  }
  FastLED.delay(150);
}

void death(){
  BRIGHTNESS = 254;
  switch(colorNum%2){
    case 0:
    allAsOne(CRGB(255,255,255));
    break;

    case 1:
    allAsOne(CRGB(0,0,0));
    break;
  }
  colorNum++;
}

void brighten(){
  if(BRIGHTNESS < 245){
    BRIGHTNESS += 10;
    FastLED.setBrightness(BRIGHTNESS);
  }
}

void dim(){
  if(BRIGHTNESS > 10){
    BRIGHTNESS -= 10;
    FastLED.setBrightness(BRIGHTNESS);
  }
}

void pauseLights(){FastLED.delay(50);}

void rgbFlash(){
    allAsOne(color[colorNum%3]);
    colorNum++;
    FastLED.delay(10000 / FRAMES_PER_SECOND);
}

void rainFlash(){
    allAsOne(CHSV(gHue, 255, BRIGHTNESS));
    gHue += 15;
    FastLED.delay(4000 / FRAMES_PER_SECOND);
}

void sine(){
  fadeToBlackBy( ledsLeft, NUM_LEDS_PER_STRIP, 20);
  int pos1 = beatsin16(10,0,NUM_LEDS_PER_STRIP/2);
  int pos2 = beatsin16(10,NUM_LEDS_PER_STRIP/2,NUM_LEDS_PER_STRIP);
  ledsLeft[pos1] += CHSV( gHue, 255, 192);
  ledsLeft[pos2] += CHSV( gHue, 255, 192);
  gHue++;
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void singleColorChange(){
    allAsOne(color[colorNum%9]);
    colorNum++;
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}



