#ifndef Lights_h
#define Lights_h

#include "Arduino.h"
#include "FastLED.h"

class Lights
{
  public:
    //CRGB color[];
    //CRGB colorAmerica[];
    CRGB* leds;
    int colorNum;
    int gHue;
    int _brightness;
    int _numLeds;
    int FRAMES_PER_SECOND;
    boolean dimming;
    
    Lights(int numLeds, int Brightness);
    void help();
    void allAsOne(CRGB);
    void allRed();
    void allBlue();
    void allGreen();
    void breathRainbow();
    void brighten();
    void cycleColors();
    void DEATH();
    void dim();
    void flashRGB();
    void flashRainbow();
    void flashAmerican();
    void pause();
    void sineWave();    
    
};


#endif

