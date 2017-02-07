#include "FastLED.h"
#include "Lights.h"
FASTLED_USING_NAMESPACE
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN1    4
#define DATA_PIN2    5
#define INPUT_PIN    9
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS1    64
#define NUM_LEDS2    64

char rxChar = '1';
char preChar = '1';


Lights *lights1 = new Lights(NUM_LEDS1, 100);

void setup() {
  delay(3000); 
  Serial.begin(9600);
  Serial.setTimeout(40);
  lights1->help();

    FastLED.addLeds<LED_TYPE, DATA_PIN1, COLOR_ORDER>(lights1->leds, NUM_LEDS1).setCorrection(TypicalLEDStrip);

}

void loop() {
  // put your main code here, to run repeatedly:
{
  if (Serial.available() > 0){
    rxChar = Serial.read();
    Serial.flush();
  }
    
    switch(rxChar){
      
    case '0':                          // If received a ?:
    lights1->help();                   // print the command list.
    rxChar = preChar;
    break;
    
    case '1':                 //Set the lights to red
    lights1->allRed();
    break;

    case '2':                 //Set the lights to Green
    lights1->allAsOne(CRGB::Green);
    break;
    
    case '3':                 //Set the lights to Blue
    lights1->allAsOne(CRGB::Blue);
    break;

    case '4':       
    lights1->flashRGB();      
    break;
    
    case '5':                 //Use sine for moving lights
    lights1->sineWave();
    break;

    case '6':                 //BreathFunction
    lights1->breathRainbow();
    break;

    case '7':                 //Dim the lights
    lights1->dim();
    rxChar = preChar;
    break;

    case '8':                 //Brighten the lights
    lights1->brighten();
    rxChar = preChar;
    break;

    case '9':
    lights1->flashRainbow();
    break;
    
    case 'a':                 //Set to a single color change through list of colors
    //lights1->
    break;

    case 'A':                 //Flash Red, White, and Blue
    lights1->flashAmerican();
    break;
    
    case 'D':                 //Activate Death (Do not use if you have epilepsy(?))
    lights1->DEATH();
    break;

    case 'p':                 //Pause the lights
    lights1->pause();
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
      //Serial.println("Time Test");
      FastLED.delay(1000 / lights1->FRAMES_PER_SECOND);
      if(lights1->FRAMES_PER_SECOND > 120){
          lights1->FRAMES_PER_SECOND = 120; 
      }

}
}
