#include "Arduino.h"
#include "Lights.h"
#include "FastLED.h"

int colorNum = 0;
int gHue = 0;
int currLED = 0;
int _brightness;
int FRAMES_PER_SECOND = 120;
boolean dimming = false;
uint8_t gCurrentPatternNumber = 0;

Lights::Lights(int numLeds, int brightness){
  leds = new CRGB[numLeds];
  _brightness = brightness;
  _numLeds = numLeds;
}

void Lights::help(){
    Serial.println("--- Command list: ---");
    Serial.println("0 -> Print this HELP");
    Serial.println("1 -> Make LEDS Red");
    Serial.println("2 -> Make LEDS Green");
    Serial.println("3 -> Make LEDS Blue");
    Serial.println("4 -> Make it turn to a set color");
    Serial.println("5 -> Make it go to a Sine");
    Serial.println("6 -> Make it Breath rainbow");
    Serial.println("7 -> Dim");
    Serial.println("8 -> Brighten");
    Serial.println("9 -> Make it flash rainbow");
    Serial.println("q -> randHSVSpin");
    Serial.println("r -> randHSVChase");
    Serial.println("s -> spinningRainbow");
    Serial.println("C -> Chaser");
    Serial.println("D -> Toggle on and off");
    Serial.println("F -> Make it show firing signal");
    Serial.println("R -> Make it show ready to fire signal");
    Serial.println("S -> Make cycle between main commands");
}


void Lights::allAsOne(CRGB color){
  for(int i = 0; i<_numLeds; i++){
    leds[i] = color;
  }
  //FastLED.show();
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void Lights::allRed(){
  for(int i = 0; i<_numLeds; i++){
    leds[i] = CRGB::Red;
  }
}



void Lights::allBlue(){
  for(int i = 0; i<_numLeds; i++){
    leds[i] = CRGB::Blue;
  }
}



void Lights::allGreen(){
  for(int i = 0; i<_numLeds; i++){
    leds[i] = CRGB::Green;
  }
}




void Lights::breathRainbow(){
    fill_rainbow(leds, _numLeds, 0);
    if(dimming){
      dim();
      if(_brightness<15){dimming = false;}
    } else if(!dimming){
      brighten();
      if(_brightness>240){dimming = true;}
    }
  FastLED.delay(150);
}




void Lights::brighten(){
  if(_brightness < 245){
    _brightness += 10;
    FastLED.setBrightness(_brightness);
  }
}



void Lights::chaser(CRGB color){
  fadeToBlackBy( leds, _numLeds, 20);
  leds[currLED%_numLeds] = color;
  FastLED.setBrightness(255);
  currLED++;
}



void Lights::cycleCommandsMain(){
  EVERY_N_SECONDS( 5 ) { 
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % 6;  //cycles throught the switch statement, do % (numberInArray + 1)
  }
  switch(gCurrentPatternNumber){
    case 0:
    allRed();
    break;

    case 1:
    allGreen();
    break;

    case 2:
    allBlue();
    break;

    case 3:
    breathRainbow();
    break;

    case 4:
    sineWave();
    break;

    case 5:
    flashRainbow();
    _brightness = 254;
    break;

    case 6:
    break;

    case 7:
    break;

    case 8:
    break;

    case 9:
    break;
  }
}



void Lights::DEATH(){
  _brightness = 254;
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




void Lights::dim(){
  if(_brightness > 10){
    _brightness -= 10;
    FastLED.setBrightness(_brightness);
  }
}



/*void Lights::flashRGB(){
  allAsOne(color[colorNum%3]);
    colorNum++;
    FastLED.delay(10000 / FRAMES_PER_SECOND);
}*/



void Lights::flashRainbow(){
  allAsOne(CHSV(gHue, 255, _brightness));
  gHue += 1;  
}



void Lights::firingMain(){
  
}



void Lights::fireReadyMain(){
  
}



void Lights::pause(){
  FastLED.delay(50);
}



void Lights::toggle(){
  if(_brightness<=3){
    _brightness = 100;
    FastLED.setBrightness(100);
  }else{
    _brightness = 0;  
    FastLED.setBrightness(0);
  }
}



void Lights::randomHSVChase(){
  fadeToBlackBy( leds, _numLeds, 20);
  leds[currLED%_numLeds] = CHSV( random8(), 255, 192 );
  currLED++;
}



void Lights::randomHSVSpin(){
   for(int i = _numLeds - 1; i >= 0; i--){
    leds[i] = leds[i-1];
  }
  leds[0] = CHSV( random8(), 255, 192 );
}



void Lights::sineWave(){
  fadeToBlackBy( leds, _numLeds, 20);
  int pos1 = beatsin16(10,0,_numLeds/2);
  int pos2 = beatsin16(10,_numLeds/2,_numLeds);
  leds[pos1] += CHSV( gHue, 255, 192);
  leds[pos2] += CHSV( gHue, 255, 192);
  gHue++;
}



void Lights::spinningRainbow(){
  for(int i = _numLeds - 1; i >= 0; i--){
    leds[i] = leds[i-1];
  }
  leds[0] = CHSV( gHue++, 255, 192 );
}





