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
    uint8_t gCurrentPatternNumber;

    
    Lights(int numLeds, int Brightness);
    void help();
    void allAsOne(CRGB);  //Makes the leds turn to the specified color
    void allRed();        //Makes the leds turn red
    void allBlue();       //Makes the leds turn blue
    void allGreen();      //Makes the leds turn green
    void breathRainbow(); //Makes the leds breath a rainbow
    void brighten();      //Makes the leds brighter
    void cycleColors();   //Makes the leds cycle between a list of colors
    void cycleCommandsMain();         //Makes the leds cycle through list of commands (Main lights)
    void DEATH();         //Makes the leds flash bright white and off
    void dim();           //Makes the leds dimmer
    //void flashRGB();      //Makes the leds flash between red, green, and blue
    void flashRainbow();  //Makes the leds flash through the colors of the rainbow
    void flashAmerican(); //Makes the leds flash through red, white, and blue
    void firingMain();    //Makes the leds show firing signal (Main lights)
    void fireReadyMain();  //Makes the leds ready to fire signal (Main lights)
    void pause();         //Makes the leds pause
    void sineWave();      //Makes the leds move in a sin wave while switching colors through HSV values
    
};


#endif

