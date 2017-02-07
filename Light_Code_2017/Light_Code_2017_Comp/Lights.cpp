#include "Arduino.h"
#include "Lights.h"
#include "FastLED.h"

CRGB color[] = {CRGB::Red, CRGB::Green, CRGB::Blue};
CRGB colorAmerica[] = {CRGB::Red, CRGB::White, CRGB::Blue};

int colorNum = 0;
int gHue = 0;
int _brightness;
int FRAMES_PER_SECOND = 120;
boolean dimming = false;

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
    Serial.println("4 -> Make it flash RGB");
    Serial.println("5 -> Make it go to a Sine");
    Serial.println("6 -> Make it Breath rainbow");
    Serial.println("7 -> Dim");
    Serial.println("8 -> Brighten");
    Serial.println("9 -> Make it flash rainbow");
    Serial.println("a -> Make it cycle one color(Currently Unusable)");
    Serial.println("A -> Make it flash American");
    Serial.println("D -> Make DEATH!!!");
}


void Lights::allAsOne(CRGB color){
  for(int i = 0; i<_numLeds; i++){
    leds[i] = color;
  }
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
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



void Lights::cycleColors(){
  
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



void Lights::flashRGB(){
  allAsOne(color[colorNum%3]);
    colorNum++;
    FastLED.delay(10000 / FRAMES_PER_SECOND);
}



void Lights::flashRainbow(){
  allAsOne(CHSV(gHue, 255, _brightness));
  gHue += 15;
  FastLED.delay(4000 / FRAMES_PER_SECOND);
}


void Lights::flashAmerican(){
  allAsOne(colorAmerica[colorNum%3]);
  colorNum++;
  FastLED.delay(10000 / FRAMES_PER_SECOND);
}



void Lights::pause(){
  FastLED.delay(50);
}



void Lights::sineWave(){
  fadeToBlackBy( leds, _numLeds, 20);
  int pos1 = beatsin16(10,0,_numLeds/2);
  int pos2 = beatsin16(10,_numLeds/2,_numLeds);
  leds[pos1] += CHSV( gHue, 255, 192);
  leds[pos2] += CHSV( gHue, 255, 192);
  gHue++;
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
